#include <Vextr/utils/Unicode.hpp>
#include <utf8proc.h>

namespace vextr::utils::unicode {

int displayWidth(uint32_t codepoint) {
  // Basic rule: use utf8proc width if possible
  int w = utf8proc_charwidth(static_cast<utf8proc_int32_t>(codepoint));

  // utf8proc returns -1 for non-printables -> treat as 0
  if (w < 0)
    return 0;

  return w;
}

int stringWidth(const std::string &utf8) {
  int width = 0;

  const utf8proc_uint8_t *str =
      reinterpret_cast<const utf8proc_uint8_t *>(utf8.data());

  utf8proc_int32_t cp;
  utf8proc_ssize_t i = 0;

  while (i < (utf8proc_ssize_t)utf8.size()) {
    i += utf8proc_iterate(str + i, -1, &cp);
    if (i <= 0)
      break;

    int w = utf8proc_charwidth(cp);
    if (w > 0)
      width += w;
  }

  return width;
}

uint32_t nextCodepoint(const std::string &s, size_t &i) {
  const utf8proc_uint8_t *data =
      reinterpret_cast<const utf8proc_uint8_t *>(s.data());

  utf8proc_int32_t cp = 0;

  utf8proc_ssize_t step = utf8proc_iterate(data + i, -1, &cp);

  if (step <= 0) {
    i++;
    return 0xFFFD;
  }

  i += step;
  return static_cast<uint32_t>(cp);
}

std::string encode(uint32_t cp) {
  utf8proc_uint8_t out[4];
  utf8proc_ssize_t len =
      utf8proc_encode_char(static_cast<utf8proc_int32_t>(cp), out);

  if (len <= 0)
    return std::string("�");

  return std::string(reinterpret_cast<char *>(out), len);
}

} // namespace vextr::utils::unicode