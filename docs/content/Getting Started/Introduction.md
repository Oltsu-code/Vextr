introduction.md

Presenting file(s)...
# Introduction

Vextr is a widget-based terminal UI framework for modern C++. It lets you
build interactive terminal apps - layouts, styling, events, navigation -
with an API that stays out of your way.

```cpp
auto button = std::make_shared<Button>("Save");
button->setStyle(Style{ .bg = {31, 189, 0} });
button->setFocusedStyle(Style{ .bg = {0, 255, 68} });
button->setOnClick([&]() { save(); });
```

That's a styled, focusable, interactive button in four lines.

## What Vextr gives you

- **Layouts** - stack, grid, and absolute positioning with per-child size constraints
- **Widgets** - labels, buttons, inputs, text areas, scroll views, dropdowns, modals, and more
- **Styling** - RGB colors, bold/italic/underline, padding, margins, and five border styles
- **Events** - keyboard input, event bubbling, and per-widget listeners
- **Navigation** - multi-screen navigator with overlays and dialogs
- **Cross-platform** - Windows and Unix terminals, out of the box
- 
## Requirements

- C++23 compiler (GCC 13+, Clang 16+, MSVC 2022+)
- CMake 3.20+
- Windows or Unix terminal

## Installation

Add this to your `CMakeLists.txt`:

```cmake
include(FetchContent)
FetchContent_Declare(
    vextr
    GIT_REPOSITORY https://github.com/Oltsu-code/Vextr.git
    GIT_TAG v0.1.0 # check latest release from https://github.com/Oltsu-code/Vextr
)
FetchContent_MakeAvailable(vextr)
target_link_libraries(my_app PRIVATE vextr) # change my_app to the actual project in your CMakeLists
```

Then include the library in your code:

```cpp
#include <Vextr/Vextr.hpp>
```