
#include <iostream>

#include "cppli.h"
#include "arg_parsing.h"

using namespace cppli::detail;
namespace cppli {
    namespace detail {
        bool current_command_is_leaf_;
    }
    bool current_command_is_leaf() {
        return detail::current_command_is_leaf_;
    }
   /* void run(int argc, const char* const* const argv) {

    }*/
}