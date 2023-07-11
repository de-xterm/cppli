#pragma once

#include <string>

namespace cppli {
    enum minor_error_behavior {
        SILENT,
        MESSAGE,
        THROW
    };

    void print_throw_or_do_nothing(minor_error_behavior behavior,
                                   const std::string& if_error_or_mesasge,
                                   const std::string& only_if_message);

    extern minor_error_behavior unrecognized_flag_behavior,
                                flag_included_multiple_times_behavior,
                                flag_given_an_argument,
                                optional_included_multiple_times_with_same_argument_behavior,
                                optional_included_multiple_times_with_different_arguments_behavior,
                                excess_positionals_behavior;
}