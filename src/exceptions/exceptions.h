#pragma once

#include <stdexcept>
#include <variant>

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
        std::variant<minor_error_type,
                major_error_type> error_variant_;

    public:
        user_error(const std::string& what, minor_error_type e);
        user_error(const std::string& what, major_error_type e);
        user_error(const std::string& what, const error_variant_t& e);

        const error_variant_t& error_type() const;
    };

    //source: https://stackoverflow.com/a/62708827
    // A trait to check that T is one of 'Types...'
    template <typename T, typename...Types>
    struct is_one_of final : std::disjunction<std::is_same<T, Types>...> {};

    template<typename... Types, typename T>
    auto operator==(const std::variant<Types...>& v, T const& t) noexcept
    -> std::enable_if_t<is_one_of<T, Types...>::value, bool>
    {
        return std::get<T>(v) == t;
    }
}