#include <Vextr/core/Context.hpp>

namespace vextr::core {

Context& Context::get() {
    static Context instance;
    return instance;
}

} // vextr::core
