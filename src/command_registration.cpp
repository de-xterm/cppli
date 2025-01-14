#include <format>

#include "iro.h"

#include "command_registration.h"

namespace cppli::detail {
    void default_help_callback(const command_context_t& cppli_current_command,

                               const variadic<std::string, string_conversion_t<std::string>, false, "subcommand name", "The name of the subcommand to print help for."
                                                                                                                "If no subcommand is provided, then help is printed for the parent command">&, std::vector<std::string> subcommand_name,

                               const flag<"name-only", "only print subcommand names">&, bool name_only,
                               const flag<"name-and-description", "print subcommand name and description">&, bool name_and_description,
                               const flag<"name-and-args", "print subcommand name and args">&, bool name_and_args,
                               const flag<"name-description-and-args", "print subcommand name, description, and args">&, bool name_description_and_args,
                               const flag<"verbose", "print current command name and description", 'v'>&, bool verbose,
                               const flag<"hide-help", "don't show help when printing subcommands">&, bool hide_help,
                               const flag<"show-help", "do show help when printing subcommands">&, bool show_help,

                               const flag<"subcommands-name-only", "only print subcommand names">&, bool subcommands_name_only,
                               const flag<"subcommands-name-and-description", "print subcommand name and description">&, bool subcommands_name_and_description,
                               const flag<"subcommands-name-and-args", "print subcommand name and args">&, bool subcommands_name_and_args,
                               const flag<"subcommands-name-description-and-args", "print subcommand name, description, and args">&, bool subcommands_name_description_and_args,
                               const flag<"subcommands-verbose", "print subcommand name and description">&, bool subcommands_verbose,

                               const option<unsigned, string_conversion_t<unsigned>, false, "recursion", "how many levels of nested subcommands to print. 0 prints none", "unsigned integer", true, false, 'r'>&, const std::optional<unsigned>& recursion) {

        extern command_name_t last_subcommand_;

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
            std::cerr << "\nboth hide help and show help were provided. Help will be shown\n";
        }

        if(subcommand_name.size()) {
            subcommand_name.insert(subcommand_name.begin(), "MAIN");
            if(!subcommand_name_to_docs().contains(subcommand_name)) {
                std::cerr << iro::bright_red << iro::effect_string(iro::bold|iro::underline, "Error:") << std::format(R"( "{}" does not refer to a valid command)" "\n", to_string(subcommand_name, " "));
                return;
            }
            get_print_documentation_string_callback()(subcommand_name, top_level_verbosity, subcommand_verbosity, recursion.value_or(default_help_recursion_level), (default_hide_help_status || hide_help) && !show_help);
        }
        else {
            get_print_documentation_string_callback()(last_subcommand_, top_level_verbosity, subcommand_verbosity, recursion.value_or(default_help_recursion_level), (default_hide_help_status || hide_help) && !show_help);
        }
    }
}
