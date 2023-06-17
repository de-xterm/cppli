
#include "cppli.h"
#include "arg_parsing.h"

using namespace cppli::detail;
namespace cppli {
    void run(int argc, const char* const* const argv) {
        auto commands_vec = parse(argc, argv);

        for(unsigned i = 0; i < commands_vec.size(); ++i) {
            if((subcommand_name_to_func().contains(commands_vec[i].name))) {
                (subcommand_name_to_func()[commands_vec[i].name])(commands_vec[i]);
            }
        }
    }
}