
#include <iostream>

#include "cppli.h"
#include "arg_parsing.h"

using namespace cppli::detail;
namespace cppli {
    void run(int argc, const char* const* const argv) {
        auto commands_vec = parse(argc, argv);

        bool runnable_command_found = false;
        for(unsigned i = 1; i < commands_vec.size(); ++i) {
            if((subcommand_name_to_func().contains(commands_vec[i].name))) {
                runnable_command_found = true;
                (subcommand_name_to_func()[commands_vec[i].name])(commands_vec[i]);
            }
        }

        if(!runnable_command_found) {
            std::cerr << "The input did not form any runnable commands\n";
        }
    }
}