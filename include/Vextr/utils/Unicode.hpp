#pragma once
#include <cstdint>
#include <string>

namespace vextr::utils::unicode {

// returns how many terminal columns a codepoint takes (1 or 2)
int displayWidth(uint32_t codepoint);

// returns total display width of a utf8 string
int stringWidth(const std::string &utf8);

uint32_t nextCodepoint(const std::string &utf8, size_t &i);

std::string encode(uint32_t cp);

} // namespace vextr::utils::unicode