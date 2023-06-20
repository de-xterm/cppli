
#include "parameter_types.h"

namespace cppli {
    std::ostream& operator<<(std::ostream& os, const string_t& s) {
        os << static_cast<const std::string&>(s);
        return os;
    }
}