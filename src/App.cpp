#include <Vextr/App.hpp>
#include <Vextr/core/Context.hpp>
#include <Vextr/utils/Debug.hpp>

#if defined(_WIN32)
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
    #include <signal.h>
#endif

namespace vextr {

// flag set by SIGWINCH on unix
#if !defined(_WIN32)
static volatile bool resizePending = false;
static void onSigwinch(int) { resizePending = true; }
#endif

App::App() {}

App::~App() {
    terminal.restore();
}

void App::setRoot(std::shared_ptr<core::Widget> root) {
    this->root = root;
}

void App::run() {
    if (!root) return;

    terminal.setup();

#if !defined(_WIN32)
    signal(SIGWINCH, onSigwinch);
#endif

    running = true;
    handleResize();

    while (running) {
        tick();
    }
}

void App::tick() {
#if !defined(_WIN32)
    if (resizePending) {
        resizePending = false;
        handleResize();
    }
#else
    // check frame size every tick cz windows is bad
    core::Size s = terminal.terminalSize();
    if (s.width != currentSize.width || s.height != currentSize.height) {
        handleResize();
    }
#endif

    int key = pollInput();
    if (key != -1) {
        if (key == 'q') { running = false; return; }
        core::Event e;
        e.type = core::EventType::Key;
        e.key  = key;
        utils::debugLog(std::to_string(key), this);
        if (root) root->onEvent(e);
    }

    if (root) {
        buffer->clear();
        renderer.render(*root, *buffer);
        renderer.present(*buffer, terminal);
    }
}

void App::handleResize() {
    currentSize = terminal.terminalSize();
    core::Context::get().terminalSize = currentSize;
    buffer = std::make_unique<backend::Buffer>(currentSize.width, currentSize.height);

    if (root) {
        root->layout(0, 0, currentSize.width, currentSize.height);
    }

    terminal.clear();
    terminal.clear_scrollback();
}

int App::pollInput() {
#if defined(_WIN32)
    if (_kbhit()) return _getch();
    return -1;
#else
    unsigned char c;
    int n = read(STDIN_FILENO, &c, 1);
    if (n == 1) return (int)c;
    return -1;
#endif
}

} // vextr

