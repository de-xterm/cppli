
#include <iostream>

#include "configuration.h"
#include "user_error.h"

namespace cppli {
    namespace detail {
        void print_throw_or_do_nothing(minor_error_type error_type,
                                       const std::string& print_if_error_or_mesasge,
                                       const std::string& print_only_if_message) {
            if(minor_error_behavior(error_type) == THROW) {
                throw user_error(print_if_error_or_mesasge, error_type);
            }
            else {
                std::cerr << print_if_error_or_mesasge << print_only_if_message;
            }
        }
    }

    error_behavior& minor_error_behavior(minor_error_type error_type) {
        static std::unordered_map<minor_error_type, error_behavior> minor_error_code_to_behavior;

        if(!minor_error_code_to_behavior.size()) {
            for(unsigned i = 0; i < NUMBER_OF_MINOR_ERROR_TYPES; ++i) {
                minor_error_code_to_behavior.emplace(static_cast<minor_error_type>(i), MESSAGE);
            }
        }

        return minor_error_code_to_behavior.at(error_type);
    }
}