#pragma once

#include <stdexcept>
#include <variant>
#include <cstdint>

#include "template_utils.h"

#include "error.h"

namespace cppli {
    namespace detail {
        using error_enum_underlying_t = uint_fast8_t;

    }
    enum minor_error_type : detail::error_enum_underlying_t {
        UNRECOGNIZED_FLAG,
        INVALID_FLAG,
        FLAG_INCLUDED_MULTIPLE_TIMES,
        FLAG_GIVEN_AN_ARGUMENT,
        OPTION_INCLUDED_MULTIPLE_TIMES,
        EXCESS_POSITIONAL,
        EXCESS_POSITIONAL_WITH_SUSPICIOUS_OPTION,
        ARGUMENT_GIVEN_TO_NAMESPACE,

        NUMBER_OF_MINOR_ERROR_TYPES
    };

    enum major_error_type : detail::error_enum_underlying_t {
        STRING_CONVERSION_ERROR,
        OPTION_REQUIRED_ARGUMENT_NOT_PROVIDED,
        REQUIRED_OPTION_NOT_PROVIDED,
        REQUIRED_POSITIONAL_NOT_PROVIDED,
        NO_RUNNABLE_COMMAND_FOUND,

        NUMBER_OF_MAJOR_ERROR_TYPES
    };

                              // use namespace for clarity
class cli_error : public cppli::detail::error { // TODO: rename to cli_error?
    public:
        using error_variant_t = std::variant<minor_error_type,
                                             major_error_type>;

    private:
        error_variant_t error_variant_;

        inline static constexpr int minor_error_reserved_codes_count = 1024, // error codes [-1,   -1024] are reserved for minor error
                                    major_error_reserved_codes_count = 1024; // error codes [-1025,-2048] are reserved for major errors

    public:
        cli_error(const std::string& what, minor_error_type e);
        cli_error(const std::string& what, major_error_type e);
        cli_error(const std::string& what, const error_variant_t& e);


        const error_variant_t& error_type() const;
    };

    std::ostream& print_fancy_error(std::ostream& os, const std::string& what);

    /// same as what(), but the printed string has pretty effects and includes an "Error: " prefix
    std::ostream& operator<<(std::ostream& os, const cli_error& err);



    namespace detail {
        template<typename T, typename...variant_ts>
        bool is_a(const std::variant<variant_ts...>& v) {
            return v.index() ==
                   detail::get_type_index_in_pack_v<std::remove_cvref_t<T>, std::remove_cvref_t<variant_ts>...>;
        }

        //source: https://stackoverflow.com/a/62708827
        /// modified by me because it was bugged

        // A trait to check that T is one of 'Types...'
        template<typename T, typename...Types>
        struct is_one_of final : std::disjunction<std::is_same<T, Types>...> {
        };

        template<typename...Types, typename T>
        auto operator==(const std::variant<Types...>& v, T const& t) noexcept
        -> std::enable_if_t<is_one_of<T, Types...>::value, bool> { // TODO: re-implement using concepts

            return is_a<T>(v) &&
                   (std::get<T>(v) == t);
        }
        // /source

        template<typename...Types, typename T>
        bool equal(const std::variant<Types...>& v, T const& t) {
            return v == t;
        }

        template<typename...Types, typename T>
        bool equal(T const& t, const std::variant<Types...>& v) {
            return v == t;
        }
    }
}