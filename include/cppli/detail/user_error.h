#pragma once

#include <stdexcept>
#include <variant>
#include <cstdint>

#include "template_utils.h"

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

        NUMBER_OF_MAJOR_ERROR_TYPES
    };

    class user_error : public std::runtime_error {
    public:
        using error_variant_t = std::variant<minor_error_type,
                                             major_error_type>;

    private:
        error_variant_t error_variant_;

    public:
        user_error(const std::string& what, minor_error_type e);
        user_error(const std::string& what, major_error_type e);
        user_error(const std::string& what, const error_variant_t& e);

        const error_variant_t& error_type() const;
    };

    /// same as what(), but the printed string has pretty effects and includes an "Error: " prefix
    std::ostream& operator<<(std::ostream& os, const user_error& err);

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
        -> std::enable_if_t<is_one_of<T, Types...>::value, bool> {

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