
#include "command_registration.h"

namespace cppli::detail {
    void default_help_callback(const command_context_t& cppli_current_command,

                               const flag<"name-only", "only print subcommand names">&,                                  bool name_only,
                               const flag<"name-and-description", "print subcommand name and description">&,             bool name_and_description,
                               const flag<"name-and-args", "print subcommand name and args">&,                           bool name_and_args,
                               const flag<"name-description-and-args", "print subcommand name, description, and args">&, bool name_description_and_args,
                               const flag<"verbose", "print subcommand name and description", 'v'>&,                     bool verbose,
                               const flag<"hide-help", "don't show help when printing subcommands">&,                    bool hide_help,
                               const flag<"show-help", "do show help when printing subcommands">&,                       bool show_help,

                               const flag<"subcommands-name-only", "only print subcommand names">&,                                  bool subcommands_name_only,
                               const flag<"subcommands-name-and-description", "print subcommand name and description">&,             bool subcommands_name_and_description,
                               const flag<"subcommands-name-and-args", "print subcommand name and args">&,                           bool subcommands_name_and_args,
                               const flag<"subcommands-name-description-and-args", "print subcommand name, description, and args">&, bool subcommands_name_description_and_args,
                               const flag<"subcommands-verbose", "print subcommand name and description">&,                          bool subcommands_verbose,

                               const option<unsigned, conversion_t<unsigned>, false, "recursion", "how many levels of nested subcommands to print. 0 prints none", "unsigned integer", true, false, 'r'>&, const std::optional<unsigned>& recursion) {

        extern subcommand_name_t last_subcommand_;

        documentation_verbosity top_level_verbosity;
        if(verbose) {
            top_level_verbosity = NAME_DESCRIPTION_AND_ARGS_WITH_ARG_DESCRIPTIONS;
        }
        else if(name_description_and_args) {
            top_level_verbosity = NAME_DESCRIPTION_AND_ARGS;
        }
        else if(name_and_args) {
            top_level_verbosity = NAME_AND_ARGS;
        }
        else if(name_and_description) {
            top_level_verbosity = NAME_AND_DESCRIPTION;
        }
        else if(name_only) {
            top_level_verbosity = NAME_ONLY;
        }
        else {
            top_level_verbosity = default_top_level_help_verbosity;
        }

        documentation_verbosity subcommand_verbosity;
        if(subcommands_verbose) {
            subcommand_verbosity = NAME_DESCRIPTION_AND_ARGS_WITH_ARG_DESCRIPTIONS;
        }
        else if(subcommands_name_description_and_args) {
            subcommand_verbosity = NAME_DESCRIPTION_AND_ARGS;
        }
        else if(subcommands_name_and_args) {
            subcommand_verbosity = NAME_AND_ARGS;
        }
        else if(subcommands_name_and_description) {
            subcommand_verbosity = NAME_AND_DESCRIPTION;
        }
        else if(subcommands_name_only) {
            subcommand_verbosity = NAME_ONLY;
        }
        else {
            subcommand_verbosity = default_subcommand_help_verbosity;
        }

        if(hide_help && show_help) {
            std::cerr << "\nhide help and show help are mutually exclusive. Help will be shown\n";
        }

        std::cout << get_documentation_string_callback(last_subcommand_, top_level_verbosity, subcommand_verbosity, recursion.value_or(default_help_recursion_level), (default_hide_help_status || hide_help) && !show_help);
    }
}
