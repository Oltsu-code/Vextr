#pragma once
#include <Vextr/backend/Buffer.hpp>
#include <Vextr/backend/Renderer.hpp>
#include <Vextr/backend/Terminal.hpp>
#include <Vextr/core/Widget.hpp>
#include <Vextr/utils/InputParser.hpp>
#include <Vextr/utils/InputReader.hpp>
#include <memory>

namespace vextr {

/// @class App
/// @brief The main application class that manages the TUI event loop and
/// rendering.
///
/// App is the entry point for a Vextr application. It manages:
/// - The root widget and widget tree
/// - The event loop (input handling, rendering, layout)
/// - Terminal initialization and rendering
/// - Focus management
///
/// Typical usage:
/// ```cpp
/// auto app = std::make_unique<App>();
/// auto root = std::make_shared<Container>();
/// // ... add widgets to root
/// app->setRoot(root);
/// app->run(); // blocks until quit() is called
/// ```
class App {
public:
  /// @brief Creates a new App instance and initializes the terminal.
  App();

  /// @brief Destroys the App and restores the terminal.
  ~App();

  /// @brief Manages rendering output to the terminal.
  /// Provides methods to draw to the screen.
  backend::Renderer renderer;

  /// @brief Sets the root widget of the application.
  ///
  /// The root widget and its children form the widget tree that will be
  /// rendered and receive input events. This must be called before run().
  ///
  /// @param root The root widget. Must not be null.
  void setRoot(std::shared_ptr<core::Widget> root);

  /// @brief Starts the event loop and blocks until quit() is called.
  ///
  /// This initializes the terminal, then repeatedly:
  /// - Polls for input events
  /// - Processes them through the widget tree
  /// - Measures and lays out widgets
  /// - Renders to the terminal
  ///
  /// @note This function blocks until quit() is called.
  void run();

  /// @brief Stops the event loop and exits run().
  ///
  /// Can be called from input handlers or callbacks to stop the application.
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