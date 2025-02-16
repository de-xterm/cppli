
#include <iostream>

#include "iro.h"

#include "cli_error.h"

namespace cppli {
    cli_error::cli_error(const std::string& what, enum minor_error_type  e) : cppli::error(what, convert_error_enum_to_error_code(e)), error_variant_(e) {}
    cli_error::cli_error(const std::string& what, enum major_error_type  e) : cppli::error(what, convert_error_enum_to_error_code(e)), error_variant_(e) {}
    cli_error::cli_error(const std::string& what, const error_variant_t& e) : cppli::error(what, std::visit([this](const auto& e_){return convert_error_enum_to_error_code(e_);},e)), error_variant_(e) {}

    const cli_error::error_variant_t& cli_error::error_type() const {
        return error_variant_;
    }

    std::ostream& print_fancy_error(std::ostream& os, const std::string& what) {
        return os << iro::bright_red << iro::effect_string(iro::bold|iro::underlined, "Error:") << ' ' << what;
    }

    std::ostream& operator<<(std::ostream& os, const cli_error& err) {
        return print_fancy_error(os, err.what());
    }
}