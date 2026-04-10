#pragma once
#include <Vextr/core/Layout.hpp>

namespace vextr::core {

class AbsoluteLayout : public Layout {
public:
  void apply(std::vector<ChildSlot> &children, Rect inner) override;
};

} // namespace vextr::core