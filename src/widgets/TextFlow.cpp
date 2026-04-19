#include <Vextr/widgets/TextFlow.hpp>
#include <Vextr/utils/Unicode.hpp>
#include <algorithm>
#include <string>

namespace vextr::widgets::detail {

namespace {

bool isWrapSpace(uint32_t cp) { return cp == ' ' || cp == '\t'; }

int spanWidth(std::string_view text, std::size_t start, std::size_t end) {
  if (end < start) {
    end = start;
  }

  std::string source(text);
  start = std::min(start, source.size());
  end = std::min(end, source.size());

  int width = 0;
  std::size_t i = start;
  while (i < end) {
    const std::size_t cpStart = i;
    uint32_t cp = utils::unicode::nextCodepoint(source, i);
    if (i == cpStart) {
      ++i;
      cp = 0xFFFD;
    }

    if (i > end) {
      break;
    }

    width += std::max(0, utils::unicode::displayWidth(cp));
  }

  return width;
}

int lineWidth(std::string_view line) {
  return utils::unicode::stringWidth(std::string(line));
}

std::vector<VisualLine> wrapLine(std::string_view line, std::size_t lineStart,
                                 int wrapWidth, int logicalLine) {
  std::vector<VisualLine> rows;
  if (wrapWidth <= 0) {
    wrapWidth = 1;
  }

  std::string source(line);
  if (source.empty()) {
    rows.push_back({logicalLine, lineStart, lineStart, 0});
    return rows;
  }

  std::size_t segmentStart = 0;
  std::size_t i = 0;
  int segmentWidth = 0;
  bool segmentHasContent = false;
  bool trimLeadingWhitespace = false;
  std::size_t lastBreakByte = std::string::npos;
  std::size_t lastBreakNextByte = std::string::npos;
  int lastBreakWidth = 0;

  auto emit = [&](std::size_t start, std::size_t end, int width) {
    rows.push_back({logicalLine, lineStart + start, lineStart + end, width});
  };

  while (i < source.size()) {
    const std::size_t cpStart = i;
    uint32_t cp = utils::unicode::nextCodepoint(source, i);
    if (i == cpStart) {
      ++i;
      cp = 0xFFFD;
    }

    const int cpWidth = std::max(0, utils::unicode::displayWidth(cp));
    const bool whitespace = isWrapSpace(cp);

    if (!segmentHasContent && trimLeadingWhitespace && whitespace) {
      segmentStart = i;
      continue;
    }

    if (segmentHasContent && segmentWidth + cpWidth > wrapWidth) {
      if (lastBreakByte != std::string::npos && lastBreakByte > segmentStart) {
        emit(segmentStart, lastBreakByte, lastBreakWidth);
        segmentStart = lastBreakNextByte;
        i = lastBreakNextByte;
      } else {
        emit(segmentStart, cpStart, segmentWidth);
        segmentStart = cpStart;
        i = cpStart;
      }

      segmentWidth = 0;
      segmentHasContent = false;
      trimLeadingWhitespace = true;
      lastBreakByte = std::string::npos;
      lastBreakNextByte = std::string::npos;
      lastBreakWidth = 0;
      continue;
    }

    segmentHasContent = true;
    segmentWidth += cpWidth;

    if (whitespace) {
      lastBreakByte = cpStart;
      lastBreakNextByte = i;
      lastBreakWidth = segmentWidth - cpWidth;
    }
  }

  if (segmentHasContent || rows.empty()) {
    rows.push_back({logicalLine, lineStart + segmentStart,
                    lineStart + source.size(), segmentWidth});
  }

  return rows;
}

} // namespace

std::vector<VisualLine> buildVisualLines(std::string_view text,
                                         TextFlowMode mode, int wrapWidth) {
  std::vector<VisualLine> rows;
  if (wrapWidth <= 0) {
    wrapWidth = 1;
  }

  std::size_t lineStart = 0;
  int logicalLine = 0;

  while (lineStart <= text.size()) {
    const std::size_t lineEnd = text.find('\n', lineStart);
    if (lineEnd == std::string_view::npos) {
      const std::string_view line = text.substr(lineStart);
      if (mode == TextFlowMode::Wrap) {
        const auto wrapped = wrapLine(line, lineStart, wrapWidth, logicalLine);
        rows.insert(rows.end(), wrapped.begin(), wrapped.end());
      } else {
        rows.push_back(
            {logicalLine, lineStart, lineStart + line.size(), lineWidth(line)});
      }
      break;
    }

    const std::string_view line = text.substr(lineStart, lineEnd - lineStart);
    if (mode == TextFlowMode::Wrap) {
      const auto wrapped = wrapLine(line, lineStart, wrapWidth, logicalLine);
      rows.insert(rows.end(), wrapped.begin(), wrapped.end());
    } else {
      rows.push_back({logicalLine, lineStart, lineEnd, lineWidth(line)});
    }

    lineStart = lineEnd + 1;
    ++logicalLine;
  }

  if (rows.empty()) {
    rows.push_back({0, 0, 0, 0});
  }

  return rows;
}

CursorLocation locateCursor(std::string_view text, int cursor,
                            TextFlowMode mode, int wrapWidth) {
  cursor = std::clamp(cursor, 0, static_cast<int>(text.size()));
  const auto rows = buildVisualLines(text, mode, wrapWidth);

  for (std::size_t idx = 0; idx < rows.size(); ++idx) {
    const VisualLine &row = rows[idx];

    if (cursor < static_cast<int>(row.startByte)) {
      return {static_cast<int>(idx), 0, row.startByte, row.endByte, row.width};
    }

    if (cursor < static_cast<int>(row.endByte)) {
      return {static_cast<int>(idx),
              spanWidth(text, row.startByte, static_cast<std::size_t>(cursor)),
              row.startByte, row.endByte, row.width};
    }

    if (cursor == static_cast<int>(row.endByte)) {
      if (idx + 1 < rows.size() && rows[idx + 1].startByte > row.endByte) {
        const VisualLine &next = rows[idx + 1];
        return {static_cast<int>(idx + 1), 0, next.startByte, next.endByte,
                next.width};
      }

      if (idx + 1 == rows.size()) {
        return {static_cast<int>(idx), row.width, row.startByte, row.endByte,
                row.width};
      }
    }
  }

  const VisualLine &row = rows.back();
  return {static_cast<int>(rows.size() - 1), row.width, row.startByte,
          row.endByte, row.width};
}

int cursorForRowColumn(std::string_view text, int rowIndex, int column,
                       TextFlowMode mode, int wrapWidth) {
  const auto rows = buildVisualLines(text, mode, wrapWidth);
  if (rows.empty()) {
    return 0;
  }

  rowIndex = std::clamp(rowIndex, 0, static_cast<int>(rows.size()) - 1);
  column = std::max(0, column);

  const VisualLine &row = rows[static_cast<std::size_t>(rowIndex)];
  std::string source(text);
  std::size_t i = row.startByte;
  int currentWidth = 0;

  while (i < row.endByte) {
    const std::size_t cpStart = i;
    uint32_t cp = utils::unicode::nextCodepoint(source, i);
    if (i == cpStart) {
      ++i;
      cp = 0xFFFD;
    }

    const int cpWidth = std::max(0, utils::unicode::displayWidth(cp));
    if (currentWidth + cpWidth > column) {
      return static_cast<int>(cpStart);
    }

    currentWidth += cpWidth;
  }

  return static_cast<int>(row.endByte);
}

} // namespace vextr::widgets::detail
