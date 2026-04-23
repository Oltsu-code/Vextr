#!/usr/bin/env ruby

# Fortnite-style terminal battle royale (single-player vs bots)

class Player
	attr_accessor :name, :x, :y, :hp, :shield, :ammo, :weapon, :alive, :medkits

	WEAPONS = {
		pickaxe: { dmg: 10, range: 1 },
		pistol:  { dmg: 20, range: 3 },
		rifle:   { dmg: 30, range: 5 },
		shotgun: { dmg: 40, range: 2 }
	}.freeze

	def initialize(name, x, y)
		@name = name
		@x = x
		@y = y
		@hp = 100
		@shield = 0
		@ammo = 15
		@weapon = :pistol
		@medkits = 1
		@alive = true
	end

	def distance_to(other)
		(x - other.x).abs + (y - other.y).abs
	end

	def weapon_data
		WEAPONS[@weapon]
	end

	def take_damage(amount)
		return unless alive

		shield_block = [shield, amount].min
		self.shield -= shield_block
		amount -= shield_block
		self.hp -= amount
		self.alive = false if hp <= 0
	end

	def heal
		return false if medkits <= 0 || hp <= 0 || hp >= 100

		self.medkits -= 1
		self.hp = [hp + 35, 100].min
		true
	end
end

class Game
	SIZE = 12
	BOT_COUNT = 8
	LOOT_COUNT = 16

	LOOT_TABLE = [
		{ type: :ammo, amount: 10 },
		{ type: :ammo, amount: 15 },
		{ type: :shield, amount: 20 },
		{ type: :shield, amount: 30 },
		{ type: :medkit, amount: 1 },
		{ type: :weapon, weapon: :shotgun },
		{ type: :weapon, weapon: :rifle }
	].freeze

	def initialize
		@turn = 1
		@storm_radius = SIZE / 2
		@center = [SIZE / 2, SIZE / 2]
		@player = Player.new("You", rand(SIZE), rand(SIZE))
		@bots = BOT_COUNT.times.map { |i| Player.new("Bot#{i + 1}", rand(SIZE), rand(SIZE)) }
		@loot = {}
		spawn_loot
	end

	def run
		intro
		loop do
			break if game_over?

			render
			player_turn
			bots_turn
			apply_storm_damage
			shrink_storm_if_needed
			@turn += 1
		end
		finish
	end

	private

	def intro
		puts "=== TERMINAL FORTNITE ==="
		puts "Survive the storm. Eliminate all bots."
		puts "Commands: w/a/s/d, shoot, loot, heal, stats, help"
		puts "Press Enter to drop..."
		STDIN.gets
	end

	def alive_bots
		@bots.select(&:alive)
	end

	def all_alive
		[@player] + alive_bots
	end

	def game_over?
		!@player.alive || alive_bots.empty?
	end

	def finish
		if @player.alive && alive_bots.empty?
			puts "\n🏆 Victory Royale!"
		else
			puts "\n💀 Eliminated on turn #{@turn}."
		end
	end

	def render
		system("cls") || system("clear")
		puts "Turn #{@turn} | Storm radius: #{@storm_radius}"
		puts "Players left: #{all_alive.count}"
		print_stats
		puts

		(0...SIZE).each do |y|
			row = (0...SIZE).map do |x|
				tile_char(x, y)
			end
			puts row.join(" ")
		end
		puts
	end

	def tile_char(x, y)
		return "X" unless in_storm?(x, y)
		return "P" if @player.alive && @player.x == x && @player.y == y
		return "E" if alive_bots.any? { |b| b.x == x && b.y == y }
		return "L" if @loot[[x, y]]

		"."
	end

	def print_stats
		puts "#{@player.name} HP: #{@player.hp} | Shield: #{@player.shield} | Ammo: #{@player.ammo}"
		puts "Weapon: #{@player.weapon} (DMG #{@player.weapon_data[:dmg]}, RNG #{@player.weapon_data[:range]}) | Medkits: #{@player.medkits}"
		nearest = alive_bots.min_by { |b| @player.distance_to(b) }
		if nearest
			puts "Nearest enemy: #{nearest.name} at distance #{@player.distance_to(nearest)}"
		end
	end

	def player_turn
		loop do
			print "> "
			cmd = STDIN.gets&.strip&.downcase
			cmd = "" if cmd.nil?

			case cmd
			when "w", "a", "s", "d"
				move(@player, cmd)
				break
			when "shoot"
				handle_shoot(@player, alive_bots)
				break
			when "loot"
				handle_loot(@player)
				break
			when "heal"
				if @player.heal
					puts "Used medkit."
					sleep(0.7)
					break
				else
					puts "Cannot heal now."
				end
			when "stats"
				print_stats
			when "help"
				puts "w/a/s/d to move, shoot nearest enemy in range, loot current tile, heal with medkit."
			else
				puts "Invalid command."
			end
		end
	end

	def bots_turn
		alive_bots.each do |bot|
			next unless bot.alive

			if bot.distance_to(@player) <= bot.weapon_data[:range] && bot.ammo > 0
				shoot(bot, @player)
			else
				step_toward(bot, @player.x, @player.y)
				handle_loot(bot)
			end
			break unless @player.alive
		end
	end

	def handle_shoot(shooter, targets)
		target = targets.select(&:alive).min_by { |t| shooter.distance_to(t) }
		if target.nil?
			puts "No targets."
			sleep(0.7)
			return
		end

		if shooter.distance_to(target) > shooter.weapon_data[:range]
			puts "Target out of range."
			sleep(0.7)
			return
		end

		if shooter.ammo <= 0
			puts "Out of ammo."
			sleep(0.7)
			return
		end

		shoot(shooter, target)
		sleep(0.7) if shooter == @player
	end

	def shoot(shooter, target)
		shooter.ammo -= 1
		base = shooter.weapon_data[:dmg]
		dmg = (base * (0.8 + rand * 0.5)).to_i
		target.take_damage(dmg)
		puts "#{shooter.name} hit #{target.name} for #{dmg}." if shooter == @player || target == @player

		return if target.alive

		puts "#{target.name} eliminated!" if shooter == @player || target == @player
		drop_loot(target)
	end

	def move(actor, dir)
		dx, dy = case dir
						 when "w" then [0, -1]
						 when "s" then [0, 1]
						 when "a" then [-1, 0]
						 when "d" then [1, 0]
						 else [0, 0]
						 end

		actor.x = [[actor.x + dx, 0].max, SIZE - 1].min
		actor.y = [[actor.y + dy, 0].max, SIZE - 1].min
	end

	def step_toward(actor, tx, ty)
		dx = tx < actor.x ? -1 : (tx > actor.x ? 1 : 0)
		dy = ty < actor.y ? -1 : (ty > actor.y ? 1 : 0)
		if rand < 0.5
			actor.x = [[actor.x + dx, 0].max, SIZE - 1].min
		else
			actor.y = [[actor.y + dy, 0].max, SIZE - 1].min
		end
	end

	def spawn_loot
		LOOT_COUNT.times do
			x, y = rand(SIZE), rand(SIZE)
			@loot[[x, y]] = LOOT_TABLE.sample
		end
	end

	def drop_loot(actor)
		@loot[[actor.x, actor.y]] ||= LOOT_TABLE.sample
	end

	def handle_loot(actor)
		item = @loot.delete([actor.x, actor.y])
		return unless item

		case item[:type]
		when :ammo
			actor.ammo += item[:amount]
			puts "Picked ammo +#{item[:amount]}" if actor == @player
		when :shield
			actor.shield = [actor.shield + item[:amount], 100].min
			puts "Picked shield +#{item[:amount]}" if actor == @player
		when :medkit
			actor.medkits += item[:amount]
			puts "Picked medkit" if actor == @player
		when :weapon
			actor.weapon = item[:weapon]
			puts "Swapped to #{item[:weapon]}" if actor == @player
		end
		sleep(0.7) if actor == @player
	end

	def shrink_storm_if_needed
		return unless (@turn % 5).zero? && @storm_radius > 1

		@storm_radius -= 1
	end

	def in_storm?(x, y)
		cx, cy = @center
		(x - cx).abs <= @storm_radius && (y - cy).abs <= @storm_radius
	end

	def apply_storm_damage
		all_alive.each do |p|
			next if in_storm?(p.x, p.y)

			p.take_damage(8)
			puts "#{p.name} took storm damage!" if p == @player
		end
		sleep(0.7)
	end
end

Game.new.run
