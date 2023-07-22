
#include <iostream>

#include "arg_parsing.h"

using namespace cppli::detail;
namespace cppli {
    namespace detail {
        bool current_command_is_leaf_;
        subcommand_name_t last_subcommand_;
    }
    bool current_command_is_leaf() {
        return detail::current_command_is_leaf_;
    }

    const subcommand_name_t& last_subcommand() {
        return last_subcommand_;
    }
   /* void run(int argc, const char* const* const argv) {

    }*/
}