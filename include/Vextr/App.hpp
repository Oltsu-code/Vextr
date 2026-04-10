#pragma once
#include <Vextr/backend/Buffer.hpp>
#include <Vextr/backend/Renderer.hpp>
#include <Vextr/backend/Terminal.hpp>
#include <Vextr/core/Widget.hpp>
#include <Vextr/utils/InputParser.hpp>
#include <Vextr/utils/InputReader.hpp>
#include <memory>

namespace vextr {

class App {
public:
  App();
  ~App();

  backend::Renderer renderer;

  void setRoot(std::shared_ptr<core::Widget> root);
  void run();
  void quit() { running = false; }

  utils::InputReader inputReader;
  utils::InputParser inputParser;
  int escapeTimer = 0;

private:
  void tick();
  void handleResize();
  int pollInput(); // returns key code or -1

  std::shared_ptr<core::Widget> root;
  backend::Terminal terminal;
  std::unique_ptr<backend::Buffer> buffer;

  core::Size currentSize = {0, 0};
  bool running = false;
};

} // namespace vextr