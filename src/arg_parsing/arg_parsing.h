#pragma once

#include "subcommand.h"

namespace cppli::detail {

    std::vector<subcommand_args_t> parse(int argc, const char* const* const argv);
}