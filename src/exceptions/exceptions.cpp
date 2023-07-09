
#include "exceptions.h"

namespace cppli {
    user_error::user_error(const std::string& what) : std::runtime_error(what) {}
}