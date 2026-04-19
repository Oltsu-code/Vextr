#pragma once
#include <cstddef>
#include <string_view>
#include <vector>

namespace vextr::widgets {

/// @enum TextFlowMode
/// @brief Determines how text is displayed when it exceeds the available width.
enum class TextFlowMode { Scroll, Wrap };

namespace detail {

struct VisualLine {
  int logicalLine = 0;
  std::size_t startByte = 0;
  std::size_t endByte = 0;
  int width = 0;
};

struct CursorLocation {
  int row = 0;
  int column = 0;
  std::size_t rowStart = 0;
  std::size_t rowEnd = 0;
  int rowWidth = 0;
};

std::vector<VisualLine> buildVisualLines(std::string_view text,
                                         TextFlowMode mode, int wrapWidth);
CursorLocation locateCursor(std::string_view text, int cursor,
                            TextFlowMode mode, int wrapWidth);
int cursorForRowColumn(std::string_view text, int rowIndex, int column,
                       TextFlowMode mode, int wrapWidth);

} // namespace detail

} // namespace vextr::widgets
