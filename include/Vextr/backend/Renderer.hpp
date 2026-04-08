#pragma once
#include <Vextr/backend/Buffer.hpp>
#include <Vextr/core/Widget.hpp>

namespace vextr::backend {

class Renderer {
public:
    void render(core::Widget& root, Buffer& buf);
};

} // vextr::backend
