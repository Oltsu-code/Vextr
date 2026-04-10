#include <Vextr/utils/InputReader.hpp>

#if defined(_WIN32)
#include <conio.h>
#else
#include <unistd.h>
#endif

namespace vextr::utils {

void InputReader::poll(InputParser &parser) {
#if defined(_WIN32)
  while (_kbhit()) {
    unsigned char c = (unsigned char)_getch();
    parser.feed(c);
  }
#else
  unsigned char c;
  while (read(STDIN_FILENO, &c, 1) == 1)
    parser.feed(c);
#endif
}

} // namespace vextr::utils