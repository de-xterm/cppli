
#include "command_registration.h"

namespace cppli::detail {
    void default_help_callback(const flag<"name-only", "only print subcommand names">&, bool name_only,
                               const flag<"name-and-description", "print subcommand name and description">&, bool name_and_description,
                               const flag<"name-and-args", "print subcommand name and args">&, bool name_and_args,
                               const flag<"name-description-and-args", "print subcommand name, description, and args">&, bool name_description_and_args,
                               const flag<"verbose", "print subcommand name and description", 'v'>&, bool verbose,
                               const flag<"hide-help", "don't show help when printing subcommands">&, bool hide_help,
                               const flag<"show-help", "do show help when printing subcommands">&, bool show_help,

                               const option<unsigned, conversions::conversion_t<unsigned>, false, "recursion", "how many levels of nested subcommands to print. 0 prints none", "unsigned integer", true, false, 'r'>&, const std::optional<unsigned>& recursion) {

        extern subcommand_name_t last_subcommand_;

        documentation_verbosity verbosity;
        if(verbose) {
            verbosity = NAME_DESCRIPTION_AND_ARGS_WITH_ARG_DESCRIPTIONS;
        }
        else if(name_description_and_args) {
            verbosity = NAME_DESCRIPTION_AND_ARGS;
        }
        else if(name_and_args) {
            verbosity = NAME_AND_ARGS;
        }
        else if(name_and_description) {
            verbosity = NAME_AND_DESCRIPTION;
        }
        else if(name_only) {
            verbosity = NAME_ONLY;
        }
        else {
            verbosity = default_help_verbosity;
        }

        if(hide_help && show_help) {
            std::cerr << "\nhide help and show help are mutually exclusive. Help will be shown\n";
        }

        std::cout << (get_documentation_string_callback())(last_subcommand_, verbosity, recursion.value_or(default_help_recursion_level), (default_hide_help_status || hide_help) && !show_help);
    }
}
