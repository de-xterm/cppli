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

    //std::string get_documentation_string(const detail::subcommand_name_t)
}