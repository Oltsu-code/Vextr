#pragma once
#include <Vextr/core/Geometry.hpp>
#include <string_view>

namespace vextr::backend {

class Terminal {
public:
  Terminal();
  ~Terminal();

  void setup();
  void restore();

  void clear();
  void clearScrollback();

  void write(std::string_view bytes);

  core::Size terminalSize() const;
};

} // namespace vextr::backend