#pragma once
#include <Vextr/core/Style.hpp>
#include <string>
#include <vector>

namespace vextr::backend {

struct Cell {
  std::string ch = " "; // string bc utf-8
  core::Color fg = {255, 255, 255};
  core::Color bg = {0, 0, 0};
  bool bold = false;
  bool underline = false;
};

class Buffer {
public:
  Buffer(int width, int height);

  void resize(int width, int height);
  void clear();
  void copyFrom(const Buffer &other);

  void set(int x, int y, const Cell &cell);
  const Cell &get(int x, int y) const;

  int width() const { return w; }
  int height() const { return h; }

private:
  int w = 0, h = 0;
  std::vector<Cell> cells;
  Cell defaultCell;

  bool inBounds(int x, int y) const {
    return x >= 0 && y >= 0 && x < w && y < h;
  }
};

} // namespace vextr::backend