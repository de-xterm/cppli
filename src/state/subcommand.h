#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>

namespace cppli::detail {
    struct subcommand_args_t {
        std::vector<std::string> positional_args;
        std::unordered_map<std::string, std::string> options_to_values;
        std::unordered_set<std::string> flags;
    };



    using subcommand_name_t = std::vector<std::string>;

    /// if arg appended to parent_command_names forms a valid subcommand
    /// pushes back arg to parent_command_names and returns true
    bool is_valid_subcommand(subcommand_name_t& parent_command_names, const std::string& arg);

    bool subcommand_takes_flag(const subcommand_name_t& subcommand, const std::string& flag_name);
    bool subcommand_takes_option(const subcommand_name_t& subcommand, const std::string& option_name);

    bool subcommand_option_argument_is_optional(const subcommand_name_t& subcommand, const std::string& option_name);
}