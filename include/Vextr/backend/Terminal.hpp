#pragma once
#include <Vextr/backend/Buffer.hpp>
#include <Vextr/core/Geometry.hpp>

namespace vextr::backend {

class Terminal {
public:
    Terminal();
    ~Terminal();

    void setup();
    void restore();  // undo setup on exit

    void clear();
    void clear_scrollback();
    void flush(const Buffer& buf);  // render full buffer to terminal

    core::Size terminalSize() const; // queries ioctl

private:
    void moveCursor(int x, int y);
    void applyCell(const Cell& cell);
};

} // vextr::backend
