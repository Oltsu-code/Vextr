#pragma once
#include <Vextr/backend/Buffer.hpp>
#include <Vextr/backend/Terminal.hpp>
#include <Vextr/core/Widget.hpp>

namespace vextr::backend {

class Renderer {
public:
  void render(core::Widget &root, Buffer &buf);
  void present(const Buffer &buf, Terminal &terminal);

private:
  Buffer previous{0, 0};
};

} // namespace vextr::backend
