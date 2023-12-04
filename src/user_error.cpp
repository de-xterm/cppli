
#include <iostream>

#include "iro.h"

#include "user_error.h"

namespace cppli {
    user_error::user_error(const std::string& what, enum minor_error_type  e) : std::runtime_error(what), error_variant_(e) {}
    user_error::user_error(const std::string& what, enum major_error_type  e) : std::runtime_error(what), error_variant_(e) {}
    user_error::user_error(const std::string& what, const error_variant_t& e) : std::runtime_error(what), error_variant_(e) {}

    const user_error::error_variant_t& user_error::error_type() const {
        return error_variant_;
    }

    std::ostream& operator<<(std::ostream& os, const user_error& err) {
        return std::cerr << iro::bright_red << iro::effect_string(iro::bold|iro::underlined, "Error:") << ' ' << err.what();
    }
}