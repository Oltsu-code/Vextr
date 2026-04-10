#pragma once
#include <string>

namespace vextr::utils {

void debugLog(const std::string &msg);
void debugLog(const std::string &msg, const void *source);

} // namespace vextr::utils

#define VEXTR_LOG(msg) vextr::utils::debugLog(msg)