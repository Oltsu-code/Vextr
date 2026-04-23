#!/usr/bin/env ruby

require 'json'
require 'net/http'
require 'uri'

WEATHER_CODE_MAP = {
  0 => 'Clear sky',
  1 => 'Mainly clear',
  2 => 'Partly cloudy',
  3 => 'Overcast',
  45 => 'Fog',
  48 => 'Rime fog',
  51 => 'Light drizzle',
  53 => 'Drizzle',
  55 => 'Dense drizzle',
  56 => 'Freezing drizzle',
  57 => 'Freezing drizzle',
  61 => 'Slight rain',
  63 => 'Rain',
  65 => 'Heavy rain',
  66 => 'Freezing rain',
  67 => 'Heavy freezing rain',
  71 => 'Slight snow',
  73 => 'Snow',
  75 => 'Heavy snow',
  77 => 'Snow grains',
  80 => 'Rain showers',
  81 => 'Rain showers',
  82 => 'Violent rain showers',
  85 => 'Snow showers',
  86 => 'Heavy snow showers',
  95 => 'Thunderstorm',
  96 => 'Thunderstorm hail',
  99 => 'Thunderstorm hail'
}.freeze

def fail_with(message)
  puts "ERROR=#{message}"
  exit 1
end

def fetch_json(url)
  uri = URI(url)
  response = Net::HTTP.start(
    uri.host,
    uri.port,
    use_ssl: uri.scheme == 'https',
    open_timeout: 8,
    read_timeout: 12
  ) do |http|
    request = Net::HTTP::Get.new(uri)
    request['User-Agent'] = 'vextr-weather-example/1.0'
    http.request(request)
  end

  unless response.is_a?(Net::HTTPSuccess)
    raise "HTTP #{response.code} from weather service"
  end

  JSON.parse(response.body)
end

def weather_text(code)
  WEATHER_CODE_MAP.fetch(code.to_i, "Unknown(#{code})")
end

city = nil
ARGV.each_with_index do |arg, idx|
  city = ARGV[idx + 1] if arg == '--city'
end

city = city.to_s.strip
fail_with('City is required. Pass --city "City Name".') if city.empty?

begin
  geocode_url = "https://geocoding-api.open-meteo.com/v1/search?name=#{URI.encode_www_form_component(city)}&count=1&language=en&format=json"
  geocode = fetch_json(geocode_url)
  first = geocode.fetch('results', []).first
  fail_with("No location found for '#{city}'.") if first.nil?

  lat = first.fetch('latitude')
  lon = first.fetch('longitude')
  resolved_name = [first['name'], first['admin1'], first['country']].compact.join(', ')

  forecast_url = "https://api.open-meteo.com/v1/forecast?latitude=#{lat}&longitude=#{lon}&timezone=auto&forecast_days=5&current=temperature_2m,wind_speed_10m,weather_code&daily=weather_code,temperature_2m_max,temperature_2m_min"
  weather = fetch_json(forecast_url)

  current = weather.fetch('current', {})
  daily = weather.fetch('daily', {})

  temp_c = current['temperature_2m']
  wind = current['wind_speed_10m']
  code = current['weather_code']
  obs_time = current['time']

  puts 'OK'
  puts "CITY=#{resolved_name}"
  puts "TEMP_C=#{temp_c}"
  puts "WIND_KMH=#{wind}"
  puts "WEATHER=#{weather_text(code)}"
  puts "OBS_TIME=#{obs_time}"
  puts 'FORECAST:'

  dates = daily.fetch('time', [])
  maxes = daily.fetch('temperature_2m_max', [])
  mins = daily.fetch('temperature_2m_min', [])
  codes = daily.fetch('weather_code', [])

  dates.each_index do |i|
    day = dates[i]
    max_t = maxes[i]
    min_t = mins[i]
    day_code = codes[i]
    puts "#{day}|#{max_t}|#{min_t}|#{weather_text(day_code)}"
  end

  puts 'END_FORECAST'
rescue StandardError => e
  fail_with(e.message)
end