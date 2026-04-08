#pragma once

namespace vextr::core {

enum class EventType {
    Key,
    MousePress,
    MouseRelease,
    MouseMove,
    Resize,
};

enum class MouseButton {
    None,
    Left,
    Right,
    Middle,
};

struct Event {
    EventType type;

    // keyboard
    int  key   = 0;
    bool shift = false;
    bool ctrl  = false;
    bool alt   = false;

    // mouse
    int         mouseX  = 0;
    int         mouseY  = 0;
    MouseButton button  = MouseButton::None;

    // resize
    int newWidth  = 0;
    int newHeight = 0;
};

} // vextr::core
