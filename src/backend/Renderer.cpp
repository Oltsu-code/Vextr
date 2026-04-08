#include <Vextr/backend/Renderer.hpp>

namespace vextr::backend {

void Renderer::render(core::Widget& root, Buffer& buf) {
    root.render(buf);
}

} // vextr::backend
