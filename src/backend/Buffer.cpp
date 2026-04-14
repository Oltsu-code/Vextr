#include <Vextr/backend/Buffer.hpp>
#include <stdexcept>

namespace vextr::backend {

Buffer::Buffer(int width, int height) { resize(width, height); }

void Buffer::resize(int width, int height) {
  w = width;
  h = height;
  cells.assign(w * h, defaultCell);
}

void Buffer::clear() { cells.assign(w * h, defaultCell); }

void Buffer::copyFrom(const Buffer &other) {
  if (w != other.w || h != other.h) {
    resize(other.w, other.h);
  }
  cells = other.cells;
}

void Buffer::set(int x, int y, const Cell &cell) {
  if (!inBounds(x, y))
    return;
  cells[y * w + x] = cell;
}

const Cell &Buffer::get(int x, int y) const {
  if (!inBounds(x, y))
    throw std::out_of_range("Buffer::get out of bounds");
  return cells[y * w + x];
}

} // namespace vextr::backend