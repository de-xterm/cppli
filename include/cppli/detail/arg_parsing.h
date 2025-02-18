#pragma once

#include "command.h"

namespace cppli::detail {
    struct parse_ret_t {
        std::vector<command_t> subcommands;
        bool printed_help = false;
        std::optional<unsigned> help_command_index;
    };

    parse_ret_t parse(const std::vector<std::string>& arg_vec);

    std::vector<std::string> argv_to_arg_vec(int argc, const char* const* argv);
    parse_ret_t parse(int argc, const char* const* const argv);
}