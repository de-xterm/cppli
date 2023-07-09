#pragma once

#include <stdexcept>

namespace cppli {
    class user_error : public std::runtime_error {
    public:
        user_error(const std::string& what);
    };
}