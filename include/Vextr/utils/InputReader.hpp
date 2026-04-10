#pragma once
#include <Vextr/utils/InputParser.hpp>

namespace vextr::utils {

class InputReader {
public:
  // reads all available bytes this tick and feeds them to parser
  void poll(InputParser &parser);
};

} // namespace vextr::utils