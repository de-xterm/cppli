
#include <iostream>

#include "configuration.h"
#include "exceptions.h"

namespace cppli {
    void print_throw_or_do_nothing(minor_error_behavior behavior,
                                   const std::string& print_if_error_or_mesasge,
                                   const std::string& print_only_if_message) {
        if(behavior == THROW) {
            throw user_error(print_if_error_or_mesasge);
        }
        else {
            std::cerr << print_if_error_or_mesasge << print_only_if_message;
        }
    }

    minor_error_behavior unrecognized_flag_behavior                                         = MESSAGE,
                         invalid_flag_behavior                                              = MESSAGE,
                         flag_given_an_argument                                             = MESSAGE,
                         flag_included_multiple_times_behavior                              = MESSAGE,
                         optional_included_multiple_times_with_same_argument_behavior       = MESSAGE,
                         optional_included_multiple_times_with_different_arguments_behavior = MESSAGE,
                         excess_positionals_behavior                                        = MESSAGE;
}