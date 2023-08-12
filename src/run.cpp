
#include "arg_parsing.h"
#include "documentation.h"
#include "command_registration.h"

namespace cppli::detail {
    extern subcommand_name_t last_subcommand_;

    void run_impl_(int argc, const char* const* const argv) {
        {
            subcommand_name_t main_help = {"MAIN", "help"};
            if(!subcommand_name_to_func().contains(main_help)) {
                register_command<default_help_callback>(main_help, "print help for this command", true);
            }
        }

        auto parse_ret = detail::parse(argc, argv);

        if(parse_ret.help_command_index && !parse_ret.printed_help) {
            // help command is special. Skip all other execution if it is encountered
            const auto& help_command = parse_ret.subcommands[*parse_ret.help_command_index];
            (detail::subcommand_name_to_func()[help_command.name])(help_command, {true});
        }                                                                         // help is always leaf
        else if(!parse_ret.printed_help) {
            const auto& commands_vec = parse_ret.subcommands;

            #ifdef CPPLI_FULL_ERROR_CHECKING_BEFORE_RUN
                for(const auto& command : commands_vec) { // throws if any errors would occur calling the given commands, without actually calling them
                    (detail::subcommand_name_to_error_checking_func()[command.name])(command);
                }
            #endif


            bool runnable_command_found = false;

            if(!detail::subcommand_name_to_docs()[{"MAIN"}].is_namespace) {
                (detail::subcommand_name_to_func()[{"MAIN"}])(commands_vec[0], {(commands_vec.size() < 2)});
                runnable_command_found = true;
                detail::last_subcommand_ = {};
            }

            for(unsigned i = 1; i < commands_vec.size(); ++i) {
                detail::last_subcommand_ = commands_vec[i-1].name;
                if((detail::subcommand_name_to_func().contains(commands_vec[i].name))) {
                    runnable_command_found = true;
                    (detail::subcommand_name_to_func()[commands_vec[i].name])(commands_vec[i], {(i == commands_vec.size()-1)});
                }
            }

            if(!runnable_command_found) {
                std::cerr << "The input did not form any runnable commands\n";
                // TODO: print help here?
            }
        }
    }
}