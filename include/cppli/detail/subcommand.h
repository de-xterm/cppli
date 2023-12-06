#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <optional>
#include <set>
#include <cassert>
#include <iostream>

#include "parameter_types.h"

namespace cppli {
    using command_name_t = std::vector<std::string>;

    struct command_context_t {
        bool  current_command_is_leaf;
    };

    namespace detail {
        struct command_inputs_t {
            std::vector<std::string> positional_args;
            std::unordered_map<std::string, std::optional<std::string>> options_to_values;
            std::unordered_set<std::string> flags;

            bool is_empty() const;
        };


        std::string to_string(const command_name_t& name, const char* delimiter = "::");


        /// this is just boost::hash_combine, but I don't want to drag boost into this library just for one function
        template<typename T>
        std::size_t hash_combine(std::size_t& seed, const T& val) {
            return (seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
        }

        struct command_name_hash_t {
            std::size_t operator()(const command_name_t& name) const noexcept;
        };

        struct command_t {
            command_name_t name;
            command_inputs_t inputs;
        };

        using subcommand_func_t = void (*)(const command_t&, const command_context_t& command_context);
        using command_error_checking_func_t = void (*)(const command_t&);

        struct command_inputs_info_t {
            std::unordered_set<std::string> flags;
            std::unordered_map<std::string, bool> option_argument_is_optional;

            std::unordered_map<std::string, std::string> flag_or_option_long_name_to_short_name;
            std::unordered_map<char, std::string> flag_or_option_short_name_to_long_name;
        };

        std::unordered_map<command_name_t, command_inputs_info_t, command_name_hash_t>&
        subcommand_name_to_inputs_info();

        std::unordered_map<command_name_t, subcommand_func_t, command_name_hash_t>& subcommand_name_to_func();

        std::unordered_map<command_name_t, command_error_checking_func_t , command_name_hash_t>&
        subcommand_name_to_error_checking_func();


        /// if arg appended to parent_command_names forms a valid subcommand,
        /// pushes back arg to parent_command_names and returns true.
        /// Otherwise, just returns false
        bool is_valid_subcommand(command_name_t& parent_command_names, const std::string& arg);

        bool subcommand_takes_flag(const command_name_t& subcommand, const std::string& flag_name);

        bool subcommand_takes_option(const command_name_t& subcommand, const std::string& option_name);

        bool subcommand_option_argument_is_optional(const command_name_t& subcommand, const std::string& option_name);

        /// string shouldn't include the leading '-' or "--"
        void error_if_flag_or_option_already_included(const command_t& subcommand, const std::string& flag_or_option);

        void
        error_if_short_flag_or_option_already_included(const command_t& subcommand, const std::string& flag_or_option);

        bool is_namespace(const command_name_t& subcommand);

        void set_program_name_and_description(std::string&& name,
                                              std::string&& description); // todo: these should probably be in documentation.h/cpp

        const std::string& program_name();

        const std::string& program_description();

        bool main_command_is_namespace();
    }
}