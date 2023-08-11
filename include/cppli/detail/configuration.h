#pragma once

#include <string>
#include <unordered_map>

#include "user_error.h"

namespace cppli {
    enum error_behavior {
        SILENT,
        MESSAGE,
        THROW
    };

    namespace detail {
        void print_throw_or_do_nothing(minor_error_type error_type,
                                       const std::string& if_error_or_mesasge,
                                       const std::string& only_if_message = "");
    }

    error_behavior& minor_error_behavior(minor_error_type error_type);
}