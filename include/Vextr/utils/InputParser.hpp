#pragma once
#include <Vextr/core/Event.hpp>
#include <queue>
#include <vector>

namespace vextr::utils {

class InputParser {
public:
  void feed(unsigned char byte);

  bool hasEvent() const { return !events.empty(); }
  bool pendingEscape() const { return buf.size() == 1 && buf[0] == 27; }
  void flushEscape();

  core::Event nextEvent();

private:
  std::vector<unsigned char> buf;
  std::queue<core::Event> events;

  void tryParse();
  void emit(int key, bool shift = false, bool ctrl = false, bool alt = false);
};

} // namespace vextr::utils
