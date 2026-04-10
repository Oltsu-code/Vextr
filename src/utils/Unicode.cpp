#include <Vextr/utils/Unicode.hpp>

namespace vextr::utils::unicode {

int displayWidth(uint32_t cp) {
  // wide: CJK and other double-width blocks
  if ((cp >= 0x1100 && cp <= 0x115F) || (cp >= 0x2E80 && cp <= 0x303E) ||
      (cp >= 0x3040 && cp <= 0xA4CF) || (cp >= 0xAC00 && cp <= 0xD7AF) ||
      (cp >= 0xF900 && cp <= 0xFAFF) || (cp >= 0xFE10 && cp <= 0xFE1F) ||
      (cp >= 0xFE30 && cp <= 0xFE6F) || (cp >= 0xFF00 && cp <= 0xFF60) ||
      (cp >= 0xFFE0 && cp <= 0xFFE6) || (cp >= 0x1F300 && cp <= 0x1F9FF)) {
    return 2;
  }
  return 1;
}

int stringWidth(const std::string &utf8) {
  int width = 0;
  size_t i = 0;
  while (i < utf8.size()) {
    uint32_t cp = 0;
    unsigned char c = utf8[i];
    int bytes;
    if (c < 0x80) {
      cp = c;
      bytes = 1;
    } else if (c < 0xE0) {
      cp = c & 0x1F;
      bytes = 2;
    } else if (c < 0xF0) {
      cp = c & 0x0F;
      bytes = 3;
    } else {
      cp = c & 0x07;
      bytes = 4;
    }

    for (int b = 1; b < bytes && i + b < utf8.size(); ++b)
      cp = (cp << 6) | (utf8[i + b] & 0x3F);

    width += displayWidth(cp);
    i += bytes;
  }
  return width;
}

} // namespace vextr::utils::unicode
