
#include "subcommand.h"

namespace cppli {
    namespace detail {
        bool current_command_is_leaf_;
        subcommand_name_t last_subcommand_;
    }

    using namespace detail;
    bool current_command_is_leaf() {
        return detail::current_command_is_leaf_;
    }

    const subcommand_name_t& last_subcommand() {
        return last_subcommand_;
    }
}