#pragma once

#include <string>
#include <subcommand.h>

namespace cppli {
    enum documentation_verbosity {
        NAME_ONLY,
        NAME_AND_DESCRIPTION,
        NAME_AND_ARGS,
        NAME_DESCRIPTION_AND_ARGS,
        NAME_DESCRIPTION_AND_ARGS_WITH_ARG_DESCRIPTIONS
    };

    extern documentation_verbosity default_help_verbosity;
    extern unsigned default_help_recursion_level;

    namespace detail {
        struct flag_documentation_t {
            std::string name,
                        documentation;

            char short_name;

            flag_documentation_t(const std::string& name, const std::string& documentation, char short_name);

            bool operator<(const flag_documentation_t& rhs) const;
        };

        struct option_documentation_t {
            std::string type,
                        name,
                        argument_text,
                        documentation;

            char short_name;

            bool is_optional,
                 argument_is_optional;

            option_documentation_t(const std::string& type, const std::string& name, const std::string& argument_text,
                                   const std::string& documentation, char short_name, bool is_optional, bool argument_is_optional);

            bool operator<(const option_documentation_t& rhs) const;
        };

        struct positional_documentation_t {
            std::string type,
                        name,
                        documentation;

            positional_documentation_t(const std::string& type, const std::string& name, const std::string& documentation,
                                       bool optional);

            bool optional;
        };

        struct variadic_documentation_t {
            std::string type,
                        name,
                        documentation;

            variadic_documentation_t(const std::string& type, const std::string& name, const std::string& documentation);
        };

        struct subcommand_documentation_t {
            std::string  name; // this is what we're sorting by
            std::string description;

            std::set<flag_documentation_t>          flags; // using ordered set because we want to print commands alphabetically
            std::set<option_documentation_t>        options;
            std::vector<positional_documentation_t> positionals;
            std::optional<variadic_documentation_t> variadic; // not vector because only one is allowed

            std::set<std::string>          subcommands;

            bool is_namespace = true;

            subcommand_documentation_t() = default;

            subcommand_documentation_t(const std::string& name, const char* description);

            bool operator<(const subcommand_documentation_t& rhs) const;
        };

        std::unordered_map<subcommand_name_t, subcommand_documentation_t, subcommand_name_hash_t>& subcommand_name_to_docs();
        std::set<std::string>& top_level_subcommands(); // TODO: remove this if it doesn't turn out to be necessary
    }

    std::string get_documentation_string(const detail::subcommand_name_t&, documentation_verbosity verbosity, unsigned recursion);

    /// returns documentation for the main command
    std::string get_documentation_string(documentation_verbosity verbosity, unsigned max_recursion_level);

}