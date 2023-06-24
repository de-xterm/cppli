#pragma once

#include "subcommand.h"

namespace cppli::detail {
    struct parse_ret_t {
        std::vector<subcommand_t> subcommands;
        bool printed_help;
    };

    parse_ret_t parse(int argc, const char* const* const argv);
}