#pragma once



//included from file "../include/cppli\command_macros.h"


//included from file "C:\programming\hitoribooru\hitoribooru-cli\lib\cppli\single-header-amalgamation\..\include\cppli\detail/subcommand_macros.h"

namespace cppli::detail {

    /// All the recursive expansion stuff is taken from https://www.scs.stanford.edu/~dm/blog/va-opt.html

    // I'm intentionally using a weird case (cPPLI) so that code completion doesn't pick up internal macros and confuse the user
    struct evaluate_at_file_scope_dummy_t {
        template<typename T>
        evaluate_at_file_scope_dummy_t(T&&) noexcept {}
    };

    #define cPPLI_internal_PARENS ()

    #define cPPLI_internal_EXPAND(...) cPPLI_internal_EXPAND2(cPPLI_internal_EXPAND2(cPPLI_internal_EXPAND2(cPPLI_internal_EXPAND2(__VA_ARGS__))))
    /*#define cPPLI_internal_EXPAND4(...) cPPLI_internal_EXPAND3(cPPLI_internal_EXPAND3(cPPLI_internal_EXPAND3(cPPLI_internal_EXPAND3(__VA_ARGS__))))
    #define cPPLI_internal_EXPAND3(...) cPPLI_internal_EXPAND2(cPPLI_internal_EXPAND2(cPPLI_internal_EXPAND2(cPPLI_internal_EXPAND2(__VA_ARGS__))))*/
    #define cPPLI_internal_EXPAND2(...) cPPLI_internal_EXPAND1(cPPLI_internal_EXPAND1(cPPLI_internal_EXPAND1(cPPLI_internal_EXPAND1(__VA_ARGS__))))
    #define cPPLI_internal_EXPAND1(...) __VA_ARGS__

    #define cPPLI_internal_FOR_EACH(macro, ...)                                    \
          __VA_OPT__(cPPLI_internal_EXPAND(cPPLI_internal_FOR_EACH_HELPER(macro, __VA_ARGS__)))

    #define cPPLI_internal_FOR_EACH_HELPER(macro, a1, ...)                                    \
          macro(a1)                                                                           \
          __VA_OPT__(cPPLI_internal_FOR_EACH_AGAIN cPPLI_internal_PARENS (macro, __VA_ARGS__))

    #define cPPLI_internal_FOR_EACH_AGAIN() cPPLI_internal_FOR_EACH_HELPER


    #define cPPLI_internal_STRINGIFY(S) cPPLI_internal_STRINGIFY_impl(S)
    #define cPPLI_internal_STRINGIFY_impl(S) #S

    #define cPPLI_internal_STRINGIFY_WITH_COMMA(S) cPPLI_internal_STRINGIFY(S),

    #define cPPLI_internal_ONE_ARG_CAT(arg1) arg1

    #define cPPLI_internal_NOT_cPPLI_internal_ONE_ARG_CAT(...) \
        cPPLI_internal_EXPAND(cPPLI_internal_NOT_FINAL_CAT_HELPER(__VA_ARGS__))

    #define cPPLI_internal_CAT(arg1, ...)                                    \
        __VA_OPT__(cPPLI_internal_NOT_)##cPPLI_internal_ONE_ARG_CAT(arg1 __VA_OPT__(, __VA_ARGS__))

    #define cPPLI_internal_NOT_FINAL_CAT_HELPER(a1, a2, ...)                         \
          __VA_OPT__(cPPLI_internal_NOT_)##cPPLI_internal_FINAL_CAT_AGAIN cPPLI_internal_PARENS (a1##_##a2 __VA_OPT__(, __VA_ARGS__))

    #define cPPLI_internal_FINAL_CAT_HELPER(arg) arg

    #define cPPLI_internal_FINAL_CAT_AGAIN() cPPLI_internal_FINAL_CAT_HELPER
    #define cPPLI_internal_NOT_cPPLI_internal_FINAL_CAT_AGAIN() cPPLI_internal_NOT_FINAL_CAT_HELPER

    #ifdef __COUNTER__
        #define cPPLI_internal_UNIQUE_NAME(NAME) cPPLI_internal_CAT(NAME, __COUNTER__) // use counter if it's available
    #else
        #define cPPLI_internal_UNIQUE_NAME(NAME) cPPLI_internal_CAT(NAME, __LINE__)  // if not, line should suffice
    #endif

    #define cPPLI_internal_EVALUATE_AT_FILE_SCOPE(EXPR)                        \
    namespace {                                                                \
        [[maybe_unused]]\
        const ::cppli::detail::evaluate_at_file_scope_dummy_t cPPLI_internal_UNIQUE_NAME(cppli_DUMMY) = EXPR; /* NOLINT */      \
    }
}
//end of "detail/subcommand_macros.h" include

//included from file "C:\programming\hitoribooru\hitoribooru-cli\lib\cppli\single-header-amalgamation\..\include\cppli\detail/command_registration.h"


//included from file "C:\programming\hitoribooru\hitoribooru-cli\lib\cppli\single-header-amalgamation\..\include\cppli\detail\subcommand.h"

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <optional>
#include <set>
#include <cassert>
#include <iostream>


//included from file "C:\programming\hitoribooru\hitoribooru-cli\lib\cppli\single-header-amalgamation\..\include\cppli\detail\parameter_types.h"

#include <cstdint>
#include <unordered_map>
#include <optional>
#include <string>
#include <stdexcept>


//included from file "C:\programming\hitoribooru\hitoribooru-cli\lib\cppli\single-header-amalgamation\..\include\cppli\detail\constexpr_string_literal.h"

#include <algorithm>

#if defined(WIN32) || defined(_WIN32)
    #undef min // windows moment
    #undef max
#endif

namespace cppli {
    namespace detail {
        constexpr char underscore_to_hyphen(char c) {
            if (c == '_') {
                return '-';
            }
            else {
                return c;
            }
        }

        constexpr bool isuppercase(char c) {
            switch (c) {
                case 'A':
                case 'B':
                case 'C':
                case 'D':
                case 'E':
                case 'F':
                case 'G':
                case 'H':
                case 'I':
                case 'J':
                case 'K':
                case 'L':
                case 'M':
                case 'N':
                case 'O':
                case 'P':
                case 'Q':
                case 'R':
                case 'S':
                case 'T':
                case 'U':
                case 'V':
                case 'W':
                case 'X':
                case 'Y':
                case 'Z':
                    return true;
                default:
                    return false;
            }
        }

        constexpr bool islowercase(char c) {
            switch (c) {
                case 'a':
                case 'b':
                case 'c':
                case 'd':
                case 'e':
                case 'f':
                case 'g':
                case 'h':
                case 'i':
                case 'j':
                case 'k':
                case 'l':
                case 'm':
                case 'n':
                case 'o':
                case 'p':
                case 'q':
                case 'r':
                case 's':
                case 't':
                case 'u':
                case 'v':
                case 'w':
                case 'x':
                case 'y':
                case 'z':
                    return true;
                default:
                    return false;
            }
        }

        constexpr bool isletter(char c) {
            return isuppercase(c) || islowercase(c);
        }

        constexpr char tolower(char c) { // yes, this is the only way.
            switch (c) {                  // The C++ standard does not guarantee that 'a', 'b', 'c'... etc. map to contiguous integer values.
                default:
                    return c;
                case 'A':
                    return 'a';
                case 'B':
                    return 'b';
                case 'C':
                    return 'c';
                case 'D':
                    return 'd';
                case 'E':
                    return 'e';
                case 'F':
                    return 'f';
                case 'G':
                    return 'g';
                case 'H':
                    return 'h';
                case 'I':
                    return 'i';
                case 'J':
                    return 'j';
                case 'K':
                    return 'k';
                case 'L':
                    return 'l';
                case 'M':
                    return 'm';
                case 'N':
                    return 'n';
                case 'O':
                    return 'o';
                case 'P':
                    return 'p';
                case 'Q':
                    return 'q';
                case 'R':
                    return 'r';
                case 'S':
                    return 's';
                case 'T':
                    return 't';
                case 'U':
                    return 'u';
                case 'V':
                    return 'v';
                case 'W':
                    return 'w';
                case 'X':
                    return 'x';
                case 'Y':
                    return 'y';
                case 'Z':
                    return 'z';
            }
        }

        constexpr bool is_numeral(char c) {
            switch (c) {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    return true;
                default:
                    return false;
            }
        }

        /*std::string tolower_and_convert_underscores(std::string str) {
            for(auto& c : str) {
                if(c == '_') {
                    c = '-';
                }
                else {
                    c = std::tolower(c);
                }
            }

            return std::move(str);
        }*/


    }
    // source for string_literal: https://ctrpeach.io/posts/cpp20-string-literal-template-parameters/
    template<size_t N>
    struct string_literal {
        template<std::size_t N_>
        constexpr string_literal(const char (& str)[N_]) {
            std::copy(str, str + N, value);
        }

        char value[N];

        constexpr string_literal() = default;

        constexpr string_literal make_lowercase_and_convert_underscores() const {
            string_literal ret;

            for (std::size_t i = 0; i < N; ++i) {
                ret.value[i] = detail::underscore_to_hyphen(detail::tolower(value[i]));
            }

            return ret;
        }

        template<std::size_t N_>
        constexpr bool operator==(const string_literal<N_>& rhs) const {
            bool all_same = true;

            for (std::size_t i = 0; i < std::min(this->size(), rhs.size()); ++i) {
                all_same &= (value[i] == rhs.value[i]);
            }

            return all_same;
        }

        constexpr char operator[](std::size_t i) const {
            return value[i];
        }

        constexpr std::size_t size() const {
            return N;
        }

        std::string string() const {
            std::string ret;
            ret.reserve(N);

            for (std::size_t i = 0; i < N; ++i) {
                ret.push_back(value[i]);
            }
            return ret;
        }

        operator std::string() const {
            return string();
        }
    };

    template<std::size_t N>
    string_literal(const char (&)[N]) -> string_literal<N - 1>;

    namespace detail {
        template<string_literal literal>
        constexpr bool all_lowercase_numeral_or_hyphen() {
            bool all_valid = true;

            for (std::size_t i = 0; i < literal.size(); ++i) {
                all_valid &= (islowercase(literal.value[i]) ||
                              is_numeral(literal.value[i]) ||
                              (literal.value[i] == '-'));
            }

            return all_valid;
        }

        template<string_literal literal>
        constexpr bool contains_uppercase() {
            bool contains = false;

            for (std::size_t i = 0; i < literal.size(); ++i) {
                contains |= isuppercase(literal.value[i]);
            }

            return contains;
        }
    }
}
//end of "constexpr_string_literal.h" include

//included from file "C:\programming\hitoribooru\hitoribooru-cli\lib\cppli\single-header-amalgamation\..\include\cppli\detail\conversions.h"

#include <string>
#include <stdexcept>
#include <limits>
#include <filesystem>


//included from file "C:\programming\hitoribooru\hitoribooru-cli\lib\cppli\single-header-amalgamation\..\include\cppli\detail\cli_error.h"

#include <stdexcept>
#include <variant>
#include <cstdint>


//included from file "C:\programming\hitoribooru\hitoribooru-cli\lib\cppli\single-header-amalgamation\..\include\cppli\detail\template_utils.h"

#include <type_traits>

namespace cppli::detail {
    template<typename T_>
    struct type_wrapper { using T = T_; };

    template<std::size_t current_index, std::size_t desired_index, typename T, typename...Ts>
    static constexpr auto get_type_from_index_in_pack_func() {
        if constexpr((current_index == 0) && (desired_index > sizeof...(Ts))) {
            return type_wrapper<void>{};
        }
        else if constexpr(current_index == desired_index) {
            return type_wrapper<T>{};
        }
        else {
            //static_assert(sizeof...(Ts) > 0, "index out of range");
            return get_type_from_index_in_pack_func<current_index+1, desired_index, Ts...>();
        }
    }

    template<std::size_t index, typename...Ts>
    using get_type_from_index_in_pack = typename decltype(get_type_from_index_in_pack_func<0,index, Ts...>())::T;


    template<typename T, typename U, typename...Ts>
    static constexpr auto get_type_index_in_pack_func() {
        if constexpr(std::is_same_v<T, U>) {
            return 0;
        }
        else if constexpr(sizeof...(Ts) > 0) {
            return 1+get_type_index_in_pack_func<T, Ts...>();
        }
        else {
            static_assert(std::is_same_v<T, U>, "pack did not contain type");
        }
    }

    template<typename T, typename...Ts>
    static constexpr std::size_t get_type_index_in_pack_v = get_type_index_in_pack_func<T, Ts...>();
}
//end of "template_utils.h" include


//included from file "C:\programming\hitoribooru\hitoribooru-cli\lib\cppli\single-header-amalgamation\..\include\cppli\detail\error.h"


#include <stdexcept>

namespace cppli {
    class cli_error;

    class error : public std::runtime_error {
    private:
        int error_code_;

        friend class cli_error;

        /// constructor for accessing negative errors codes. Accessed by cppli::cli_error (which is a friend)
        inline error(const std::string& what, int error_code) : std::runtime_error(what), error_code_(error_code) {}

    public:
        inline int error_code() const {
            return error_code_;
        }

        /// error code is unsigned because negative values are reserved for errors generated by cppli
        inline error(const std::string& what, uint16_t error_code) : std::runtime_error(what), error_code_(error_code) {}
        inline error(const char*        what, uint16_t error_code) : std::runtime_error(what), error_code_(error_code) {}

    };
}
//end of "error.h" include

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
class cli_error : public cppli::error { // TODO: rename to cli_error?
    public:
        using error_variant_t = std::variant<minor_error_type,
                                             major_error_type>;

    private:
        error_variant_t error_variant_;

        inline static constexpr int minor_error_reserved_codes_count = 1024, // error codes [-1,   -1024] are reserved for minor error
                                    major_error_reserved_codes_count = 1024; // error codes [-1025,-2048] are reserved for major errors

    int convert_error_enum_to_error_code(minor_error_type e) {
        return -int(e);
    }

    int convert_error_enum_to_error_code(major_error_type e) {
        return minor_error_reserved_codes_count-int(e);
    }

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
//end of "cli_error.h" include

namespace cppli {

    // TODO: I think I could move partial specialization operator() implementations into a .cpp file to speed up compile times
    template<typename T>
    struct string_conversion_t {
        T operator()(const std::string& str) const {
            static_assert(std::is_convertible_v<std::string, T> ||
                          std::is_constructible_v<T, std::string>,
                          "if no string_conversion_t partial specialization is available, then std::string must be convertible to the desired type");
            return T(str);
        }

        static constexpr string_literal type_string = T::cppli_type_string;
    };

    template<typename T>
    struct string_conversion_t<std::optional<T>> {
        std::optional<T> operator()(const std::optional<std::string>& s) const {
            if(s.has_value()) {
                if constexpr(std::is_constructible_v<T, const std::string&>) {
                    return std::optional<T>(std::in_place, *s);
                }
                else {
                    return std::optional<T>(string_conversion_t<T>()(*s));
                }
            }
            else {
                return std::nullopt;
            }
        }

        std::optional<T> operator()(const std::string& s) const {
            if constexpr(std::is_constructible_v<T, const std::string&>) {
                return std::optional<T>(std::in_place, s);
            }
            else {
                return std::optional<T>(string_conversion_t<T>()(s));
            }
        }

        static constexpr string_literal type_string = string_conversion_t<T>::type_string;
    };

    template<>
    struct string_conversion_t<int> {
        int operator()(const std::string& str) const {
            try {
                return std::stoi(str);
            }
            catch(std::invalid_argument& e) {
                throw cli_error("Could not form a valid integer from string \"" + str + "\"", STRING_CONVERSION_ERROR);
            }
            catch(std::out_of_range& e) {
                throw cli_error("Could not form a valid integer from string \"" + str + "\" because the resulting integer would be out of range", STRING_CONVERSION_ERROR);
            }
        }

        static constexpr string_literal type_string = "integer";
    };

    template<>
    struct string_conversion_t<unsigned> {
        int operator()(const std::string& str) const {
            unsigned long ret;
            try {
                ret = std::stoul(str);
            }
            catch(std::invalid_argument& e) {
                throw cli_error("Could not form a valid integer from string \"" + str + "\"", STRING_CONVERSION_ERROR);
            }
            catch(std::out_of_range& e) {
                throw cli_error("Could not form a valid integer from string \"" + str + "\" because the resulting integer would be out of range", STRING_CONVERSION_ERROR);
            }
            if(ret > std::numeric_limits<unsigned>::max()) {
                throw cli_error("Could not form a valid unsigned integer from string \"" + str + "\" because the resulting integer would be out of range", STRING_CONVERSION_ERROR);
            }

            return ret;
        }

        static constexpr string_literal type_string = "integer";
    };

    template<>
    struct string_conversion_t<char> {
        int operator()(const std::string& str) const {
            if(!str.size()) {
                throw cli_error("Could not form a character from the given string because it was empty", STRING_CONVERSION_ERROR);
            }

            return str[0];
        }

        static constexpr string_literal type_string = "character";
    };

    template<>
    struct string_conversion_t<float> {
        float operator()(const std::string& str) const {
            try {
                return std::stof(str);
            }
            catch(std::invalid_argument& e) {
                throw cli_error("Could not form a valid decimal from string \"" + str + "\"", STRING_CONVERSION_ERROR);
            }
            catch(std::out_of_range& e) {
                throw cli_error("Could not form a valid decimal from string \"" + str + "\" because the resulting integer would be out of range", STRING_CONVERSION_ERROR);
            }
        }

        static constexpr string_literal type_string = "decimal";
    };

    template<>
    struct string_conversion_t<std::string> {
        const std::string& operator()(const std::string& str) const {
            return str;
        }

        static constexpr string_literal type_string = "string";
    };

    template<>
    struct string_conversion_t<std::filesystem::path> {
        std::filesystem::path operator()(const std::string& str) const {
            return {str};
        }

        static constexpr string_literal type_string = "string";
    };
}
//end of "conversions.h" include

namespace cppli {

    namespace detail {
        // placeholder class
        template<string_literal name_, string_literal documentation_, char short_name_ = '\0'>
        class flag {
            static_assert(short_name_ == '\0' || isletter(short_name_), "flag short name must be a letter");
            static_assert(isletter(name_[0]), "flag long name must begin with a letter");
            static_assert(!contains_uppercase<name_>(), "flag long name cannot  contain uppercase characters");

        public:
            static constexpr auto name = name_.make_lowercase_and_convert_underscores();
            static constexpr auto short_name = short_name_;
            static constexpr auto documentation = documentation_;

            static constexpr auto has_short_name = (short_name_ != '\0');
            static constexpr auto has_long_name  = true;

            flag() = default;
        };

        template<typename type_, typename conversion_t_, bool default_construct_when_empty_, string_literal name_, string_literal documentation_, string_literal argument_text_,
                bool optional_, bool argument_optional_,
                char short_name_ = '\0'>
        class option {
            static_assert(isletter(name_[0]), "option long name must begin with a letter");
            static_assert(!contains_uppercase<name_>(), "option long name cannot  contain uppercase characters");
            static_assert(short_name_ == '\0' || isletter(short_name_), "option short name must be a letter");
            static_assert(!optional_ || std::is_constructible_v<type_, std::string> || std::is_move_constructible_v<type_>,
                          "The type parameter of an optional option must be constructible from an std::string or have a move constructor available");


        public:
            using type = type_;
            using conversion_t = conversion_t_;

            static constexpr auto default_construct_when_empty = default_construct_when_empty_;
            static constexpr auto name = name_.make_lowercase_and_convert_underscores();
            static constexpr auto type_string = conversion_t::type_string;
            static constexpr auto cppli_type_string = type_string;
            static constexpr auto short_name = short_name_;
            static constexpr auto documentation = documentation_;
            static constexpr auto argument_text = argument_text_;
            static constexpr auto optional = optional_;
            static constexpr auto argument_optional = argument_optional_;

            static constexpr auto has_short_name = (short_name_ != '\0');
            static constexpr auto has_long_name  = true;
        };

        template<typename type_, typename conversion_t_, bool default_construct_when_empty_, string_literal name_, string_literal documentation_, string_literal argument_text_,
                char short_name_>
        class option<type_, conversion_t_, default_construct_when_empty_, name_, documentation_, argument_text_,
                true, true,
                short_name_> {
            /// type_ if default_value == no_default_value, otherwise std::optional<type_>
        private:
            std::optional<type_> value_;
            bool was_included_ = false;

            static_assert(short_name_ == '\0' || isletter(short_name_), "option short name must be a letter");
            static_assert(isletter(name_[0]), "option long name must begin with a letter");
            static_assert(!contains_uppercase<name_>(), "option long name cannot  contain uppercase characters");

        public:
            using type = type_;
            using conversion_t = conversion_t_;

            static constexpr auto default_construct_when_empty = default_construct_when_empty_;
            static constexpr auto name = name_.make_lowercase_and_convert_underscores();
            static constexpr auto type_string = conversion_t::type_string;
            static constexpr auto cppli_type_string = type_string;
            static constexpr auto short_name = short_name_;
            static constexpr auto documentation = documentation_;
            static constexpr auto argument_text = argument_text_;
            static constexpr auto optional = true;
            static constexpr auto argument_optional = true;

            static constexpr auto has_short_name = (short_name_ != '\0');
            static constexpr auto has_long_name  = true;


            option(const std::optional<std::string>& str) : was_included_(true) {
                if (str.has_value()) {
                    value_ = conversion_t()(*str);
                }
                else {
                    value_ = std::nullopt;
                }
            }

            option() = default;

            option(const option&) = default;
            option(option&&) = default;

            option& operator=(const option&) = default;
            option& operator=(option&&) = default;

            const std::optional<type>& std_optional() const& {
                return value_;
            }

            operator const std::optional<type>&() const& {
                return std_optional();
            }

            explicit operator bool() const {
                return has_value();
            }

            bool was_included() const {
                return was_included_;
            }

            bool has_value() const {
                return value_.has_value();
            }

            /// call func with value if has_value() is true, does nothing otherwise
            template<typename func_t>
            void access_value_if_present(func_t&& func) const {
                if(has_value()) {
                    func(*value_);
                }
            }

            template<typename U>
            type value_or(U&& default_value) const& {
                return value_.template value_or(std::forward<U>(default_value));
            }

            template<typename U>
            constexpr type value_or(U&& default_value) && {
                return value_.template value_or(std::forward<U>(default_value));
            }

            const type_& value() const& {
                return *value_;
            }

            const type_& operator*() const {
                return value();
            }

            const type_* operator->() const {
                return &value();
            }
        };

        template<typename type_, typename conversion_t_, bool default_construct_when_empty_, bool optional_, string_literal name_, string_literal documentation_>
        class positional {
            static_assert(std::is_constructible_v<type_, std::string> || std::is_move_constructible_v<type_>,
                          "The type parameter of an optional positional argument must be constructible from an std::string or have a move constructor available");

        public:
            using conversion_t = conversion_t_;

            static constexpr auto default_construct_when_empty = default_construct_when_empty_;
            static constexpr auto name = name_;
            static constexpr auto type_string = conversion_t::type_string;
            static constexpr auto optional = true;
            static constexpr auto documentation = documentation_;

            static constexpr auto has_short_name = false;
            static constexpr auto has_long_name  = false;
        };

        template<typename type_, typename conversion_t_, bool default_construct_when_empty_, string_literal name_, string_literal documentation_>
        class positional<type_,  conversion_t_, default_construct_when_empty_, false, name_, documentation_> {
        public:
            using type = type_;
            using conversion_t = conversion_t_;

            static constexpr auto default_construct_when_empty = default_construct_when_empty_;
            static constexpr auto name = name_;
            static constexpr auto type_string = conversion_t::type_string; // TODO: delete one of these
            static constexpr auto cppli_type_string = type_string;
            static constexpr auto optional = false;
            static constexpr auto documentation = documentation_;

            static constexpr auto has_short_name = false;
            static constexpr auto has_long_name  = false;

            positional() = default;
        };
                                                            // 99% sure this is just ignored // TODO: delete
        template<typename type_, typename conversion_t_, bool default_construct_when_empty_, string_literal name_, string_literal documentation_>
        class variadic {
        public:
            using type = type_;
            using conversion_t = conversion_t_;

            static constexpr auto default_construct_when_empty = default_construct_when_empty_;
            static constexpr auto name = name_;
            static constexpr auto type_string = conversion_t::type_string; // TODO: delete one of these
            static constexpr auto cppli_type_string = type_string;
            static constexpr auto optional = false;
            static constexpr auto documentation = documentation_;

            static constexpr auto has_short_name = false;
            static constexpr auto has_long_name  = false;
        };

        template<typename T>
        struct argument_info_t {
            static constexpr bool is_raw_type = true;
            static constexpr bool is_flag = false;
            static constexpr bool is_option = false;
            static constexpr bool is_positional = false;
            static constexpr bool is_variadic = false;

            static constexpr bool has_long_name = false;
            static constexpr bool has_short_name = false;
        };

        template<string_literal name_, char short_name_, string_literal documentation_>
        struct argument_info_t<flag<name_, documentation_, short_name_>> {
            static constexpr bool is_raw_type = false;
            static constexpr bool is_flag = true;
            static constexpr bool is_option = false;
            static constexpr bool is_positional = false;
            static constexpr bool is_variadic = false;

            static constexpr bool has_long_name  = true;
            static constexpr bool has_short_name = (short_name_ != '\0');
        };

        template<typename type_, typename conversion_t_, bool default_construct_when_empty_, string_literal name_, string_literal documentation_, string_literal argument_text_,
                bool optional_, bool argument_optional_,
                char short_name_>
        struct argument_info_t<option<type_, conversion_t_, default_construct_when_empty_, name_, documentation_, argument_text_, optional_, argument_optional_, short_name_>> {
            static constexpr bool is_raw_type = false;
            static constexpr bool is_flag = false;
            static constexpr bool is_option = true;
            static constexpr bool is_positional = false;
            static constexpr bool is_variadic = false;

            static constexpr bool has_long_name = true;
            static constexpr bool has_short_name = (short_name_ != '\0');
        };

        template<typename type_, typename conversion_t_, bool default_construct_when_empty_, bool optional_, string_literal name_, string_literal documentation_>
        struct argument_info_t<positional<type_, conversion_t_, default_construct_when_empty_, optional_, name_, documentation_>> {
            static constexpr bool is_raw_type = false;
            static constexpr bool is_flag = false;
            static constexpr bool is_option = false;
            static constexpr bool is_positional = true;
            static constexpr bool is_variadic = false;

            static constexpr bool has_long_name = false;
            static constexpr bool has_short_name = false;
        };

        template<typename type_, typename conversion_t_, bool default_construct_when_empty_, string_literal name_, string_literal documentation_>
        struct argument_info_t<variadic<type_, conversion_t_, default_construct_when_empty_, name_, documentation_>> {
                static constexpr bool is_raw_type = false;
                static constexpr bool is_flag = false;
                static constexpr bool is_option = false;
                static constexpr bool is_positional = false;
                static constexpr bool is_variadic = true;

                static constexpr bool has_long_name = false;
                static constexpr bool has_short_name = false;
        };
    }
}
//end of "parameter_types.h" include

namespace cppli {
    using command_name_t = std::vector<std::string>;

    struct command_context_t {
        bool  current_command_is_leaf;
    };

    namespace detail {
        struct command_inputs_t {
            std::vector<std::string> positional_args;
            std::unordered_map<std::string, std::optional<std::string>> options_to_values;
            std::unordered_set<std::string> flags;

            bool is_empty() const;
        };


        std::string to_string(const command_name_t& name, const char* delimiter = "::");


        /// this is just boost::hash_combine, but I don't want to drag boost into this library just for one function
        template<typename T>
        std::size_t hash_combine(std::size_t& seed, const T& val) {
            return (seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
        }

        struct command_name_hash_t {
            std::size_t operator()(const command_name_t& name) const noexcept;
        };

        struct command_t {
            command_name_t name;
            command_inputs_t inputs;
        };

        using subcommand_func_t = void (*)(const command_t&, const command_context_t& command_context);
        using command_error_checking_func_t = void (*)(const command_t&);

        struct command_inputs_info_t {
            std::unordered_set<std::string> flags;
            std::unordered_map<std::string, bool> option_argument_is_optional;

            std::unordered_map<std::string, std::string> flag_or_option_long_name_to_short_name;
            std::unordered_map<char, std::string> flag_or_option_short_name_to_long_name;
        };

        std::unordered_map<command_name_t, command_inputs_info_t, command_name_hash_t>&
        subcommand_name_to_inputs_info();

        std::unordered_map<command_name_t, subcommand_func_t, command_name_hash_t>& subcommand_name_to_func();

        std::unordered_map<command_name_t, command_error_checking_func_t , command_name_hash_t>&
        subcommand_name_to_error_checking_func();


        /// if arg appended to parent_command_names forms a valid subcommand,
        /// pushes back arg to parent_command_names and returns true.
        /// Otherwise, just returns false
        bool is_valid_subcommand(command_name_t& parent_command_names, const std::string& arg);

        bool subcommand_takes_flag(const command_name_t& subcommand, const std::string& flag_name);

        bool subcommand_takes_option(const command_name_t& subcommand, const std::string& option_name);

        bool subcommand_option_argument_is_optional(const command_name_t& subcommand, const std::string& option_name);

        /// string shouldn't include the leading '-' or "--"
        void error_if_flag_or_option_already_included(const command_t& subcommand, const std::string& flag_or_option);

        void
        error_if_short_flag_or_option_already_included(const command_t& subcommand, const std::string& flag_or_option);

        bool is_namespace(const command_name_t& subcommand);

        void set_program_name_and_description(std::string&& name,
                                              std::string&& description); // todo: these should probably be in documentation.h/cpp

        const std::string& program_name();

        const std::string& program_description();

        bool main_command_is_namespace();
    }
}
//end of "subcommand.h" include

//included from file "C:\programming\hitoribooru\hitoribooru-cli\lib\cppli\single-header-amalgamation\..\include\cppli\detail\documentation.h"

#include <string>


namespace cppli {
    enum documentation_verbosity {
        NAME_ONLY,
        NAME_AND_DESCRIPTION,
        NAME_AND_ARGS,
        NAME_DESCRIPTION_AND_ARGS,
        NAME_DESCRIPTION_AND_ARGS_WITH_ARG_DESCRIPTIONS
    };

    extern documentation_verbosity default_top_level_help_verbosity;
    extern documentation_verbosity default_subcommand_help_verbosity;
    extern unsigned default_help_recursion_level;
    extern bool default_hide_help_status;

    struct flag_documentation_t {
        std::string name,
                    documentation;

        char short_name;

        flag_documentation_t(const std::string& name, const std::string& documentation, char short_name);

        bool operator<(const flag_documentation_t& rhs) const;
    };

    struct option_documentation_t {
        std::string type,
                name,
                argument_text,
                documentation;

        char short_name;

        bool is_optional,
                argument_is_optional;

        option_documentation_t(const std::string& type, const std::string& name, const std::string& argument_text,
                               const std::string& documentation, char short_name, bool is_optional, bool argument_is_optional);

        bool operator<(const option_documentation_t& rhs) const;
    };

    struct positional_documentation_t {
        std::string type,
                name,
                documentation;

        positional_documentation_t(const std::string& type, const std::string& name, const std::string& documentation,
                                   bool optional);

        bool optional;
    };

    struct variadic_documentation_t {
        std::string type,
                    name,
                    documentation;

        variadic_documentation_t(const std::string& type, const std::string& name, const std::string& documentation);
    };

    struct command_documentation_t {
        std::string name; // this is what we're sorting by
        std::string description;

        std::set<flag_documentation_t>          flags;    // using ordered set because we want to print commands sorted lexicographically
        std::set<option_documentation_t>        options;
        std::vector<positional_documentation_t> positionals;
        std::optional<variadic_documentation_t> variadic; // optional instead of vector because only one is allowed

        std::set<std::string>                   subcommands;

        bool is_namespace = true;

        command_documentation_t() = default;
        command_documentation_t(const std::string& name, const char* description);

        bool operator<(const command_documentation_t& rhs) const;
    };

    using print_documentation_string_t = void(*)(const command_name_t&,
                                                 const documentation_verbosity& top_level_verbosity, const documentation_verbosity& subcommand_verbosity,
                                                 unsigned recursion, bool hide_help);

    void default_print_documentation_string_callback(const command_name_t&,
                                                            const documentation_verbosity& top_level_verbosity, const documentation_verbosity& subcommand_verbosity,
                                                            unsigned recursion, bool hide_help);

    void                         set_print_documentation_string_callback(print_documentation_string_t callback);
    print_documentation_string_t get_print_documentation_string_callback();

    namespace detail {
        std::unordered_map<command_name_t, command_documentation_t, command_name_hash_t>& subcommand_name_to_docs();

        const command_documentation_t& get_command_docs_from_name(const command_name_t& name);
    }
}
//end of "documentation.h" include

//included from file "C:\programming\hitoribooru\hitoribooru-cli\lib\cppli\single-header-amalgamation\..\include\cppli\detail\configuration.h"

#include <string>
#include <unordered_map>


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

   // error_behavior& minor_error_behavior(minor_error_type error_type);

    void           set_minor_error_behavior(minor_error_type type, error_behavior behavior);
    error_behavior get_minor_error_behavior(minor_error_type type);
}
//end of "configuration.h" include

//included from file "C:\programming\hitoribooru\hitoribooru-cli\lib\cppli\single-header-amalgamation\..\include\cppli\detail\process_argument.h"


namespace cppli::detail {
    template<typename T, typename last, typename conversion_t_>
    T process_positional(unsigned cumulative_positional_index, const command_t& subcommand,
                         const std::string& main_command_or_subcommand, const std::string& long_name) {
        --cumulative_positional_index; // because we've already skipped over the actual parameter for the positional, cumulative_positional_index will be too big

        if constexpr(last::optional) {
            if (cumulative_positional_index < subcommand.inputs.positional_args.size()) {
                try {
                    return conversion_t_()(
                            subcommand.inputs.positional_args[cumulative_positional_index]);
                }
                catch (cli_error& e) {
                    throw cli_error("Error initializing " + main_command_or_subcommand + " \"" +
                                     to_string(subcommand.name) + "\" positional argument \"" +
                                     long_name + "\". Details: " + e.what(), e.error_type());
                }
            }
            else {
                return {}; // empty optional
            }
        }
        else {
            if ((cumulative_positional_index >= subcommand.inputs.positional_args.size())) {
                throw cli_error(main_command_or_subcommand + " \"" + to_string(subcommand.name) +
                                 "\" required positional argument \"" + long_name +
                                 "\" was not provided (expected an argument of type " +
                                 conversion_t_::type_string.string() + ')',
                                 REQUIRED_POSITIONAL_NOT_PROVIDED);
            }
            else {
                try {
                    return conversion_t_()(
                            subcommand.inputs.positional_args[cumulative_positional_index]);
                }
                catch (cli_error& e) {
                    throw cli_error("Error initializing " + main_command_or_subcommand + " \"" +
                                     to_string(subcommand.name) + "\" positional argument \"" +
                                     long_name + "\". Details: " + e.what(), e.error_type());
                }
            }
        }
    }

    template<typename T, typename last, typename conversion_t_>
    T process_variadic(unsigned cumulative_positional_index, const command_t& subcommand,
                       const std::string& main_command_or_subcommand, const std::string& long_name) {
        T ret; // T is a vector
        for (unsigned i = cumulative_positional_index;
             i < subcommand.inputs.positional_args.size(); ++i) {
            try {
                ret.emplace_back(conversion_t_()(subcommand.inputs.positional_args[i]));
            }
            catch (cli_error& e) {
                throw cli_error("Error initializing " + main_command_or_subcommand + " \"" +
                                 to_string(subcommand.name) + "\" variadic argument pack \"" +
                                 long_name + "\" at argument index " + std::to_string(i) +
                                 ". Details: " + e.what(), e.error_type());
            }
        }
        return ret;
    }

    template<typename T, typename last, typename conversion_t_>
    T process_required_option(const command_t& subcommand,
                                       const std::string& main_command_or_subcommand,
                                       const std::string& long_name, const std::optional<std::string>& short_name) {
        
        if (!subcommand.inputs.options_to_values.contains(long_name) &&
            (!short_name || !subcommand.inputs.options_to_values.contains(*short_name))) {

            throw cli_error(main_command_or_subcommand + " \"" + to_string(subcommand.name) +
                             "\" was not provided with required option \"" + long_name + '"',
                             REQUIRED_OPTION_NOT_PROVIDED);
        }
        else if ((subcommand.inputs.options_to_values.contains(long_name) &&
                  !subcommand.inputs.options_to_values.at(long_name).has_value()) ||
                 (short_name && subcommand.inputs.options_to_values.contains(*short_name) &&
                  !subcommand.inputs.options_to_values.at(*short_name).has_value())) {

            throw cli_error(
                    main_command_or_subcommand + " \"" + to_string(subcommand.name) + "\" option \""
                    + long_name + "\" requires an argument, but one was not provided "
                                       "(expected an argument of type "
                    + conversion_t_::type_string.make_lowercase_and_convert_underscores().string() +
                    ')',
                    OPTION_REQUIRED_ARGUMENT_NOT_PROVIDED);
        }
        else { // by this point, none of the optionals we're interested in are empty
            if (subcommand.inputs.options_to_values.contains(long_name)) {
                try {
                    return conversion_t_()(*subcommand.inputs.options_to_values.at(long_name));
                }
                catch (cli_error& e) {
                    throw cli_error("Error initializing " + main_command_or_subcommand + " \""
                                     + to_string(subcommand.name) + "\" option \"" +
                                     long_name + "\". Details: " + e.what(), e.error_type());
                }
            }
            else { // has_short_name is guaranteed to be true at this point
                try {
                    return conversion_t_()(*subcommand.inputs.options_to_values.at(*short_name));
                }
                catch (cli_error& e) {
                    throw cli_error("Error initializing " + main_command_or_subcommand + " \"" +
                                     to_string(subcommand.name) + "\" option \"" + *short_name +
                                     "\" (full name \"" + long_name + "\"). Details: " +
                                     e.what(), e.error_type());
                }
            }
        }
    }

    template<typename T, typename last, typename conversion_t_>
    T process_required_argument_option(const command_t& subcommand,
                                       const std::string& main_command_or_subcommand,
                                       const std::string& long_name, const std::optional<std::string>& short_name) {

        if (subcommand.inputs.options_to_values.contains(long_name)) {
            if (!subcommand.inputs.options_to_values.at(
                    long_name).has_value()) { // TODO: aren't we doing this check in arg_parsing.cpp too?
                throw cli_error(main_command_or_subcommand + to_string(subcommand.name) +
                                 "\" option \"" + long_name + "\" "
                                                                   "requires an argument, but one was not provided (expected an argument of type "
                                 +
                                 conversion_t_::type_string.make_lowercase_and_convert_underscores().string() +
                                 "."
                                 "Note that this option is optional, so it is valid to omit it entirely, "
                                 "but the option's argument is required, so if the option is provided, it must come with an argument",
                                 OPTION_REQUIRED_ARGUMENT_NOT_PROVIDED);
            }
            else {
                try {
                    return conversion_t_()(*subcommand.inputs.options_to_values.at(
                            long_name)); // no need for has_value check here; returning an empty optional is valid
                }
                catch (cli_error& e) {
                    throw cli_error(
                            "Error initializing " + main_command_or_subcommand + " \"" +
                            to_string(subcommand.name) + "\" option \"" + long_name +
                            "\". Details: " + e.what(), e.error_type());
                }
            }
        }
        else if (short_name && subcommand.inputs.options_to_values.contains(
                *short_name)) { // TODO: evaluating short_name could use if constexpr
            if (!subcommand.inputs.options_to_values.at(*short_name).has_value()) {
                throw cli_error(main_command_or_subcommand + to_string(subcommand.name) +
                                 "\" option \"" + *short_name + "\" (full name \"" +
                                 long_name + "\") "
                                                  "requires an argument, but one was not provided (expected an argument of type "
                                 + conversion_t_::type_string.string() + "."
                                                                        "Note that this option is optional, so it is valid to omit it entirely, "
                                                                        "but the option's argument is required, so if the option is provided, it must come with an argument",
                                 OPTION_REQUIRED_ARGUMENT_NOT_PROVIDED);
            }
            else {
                try {
                    return conversion_t_()(*subcommand.inputs.options_to_values.at(
                            *short_name)); // no need for has_value check here; returning an empty optional is valid
                }
                catch (cli_error& e) {
                    throw cli_error(
                            "Error initializing " + main_command_or_subcommand + " \"" +
                            to_string(subcommand.name) + "\" option \"" + *short_name +
                            "\" (full name \"" + long_name + "\"). Details: " + e.what(),
                            e.error_type());
                }
            }
        }
        else {
            return {};
        }
    }

    template<typename T, typename last, typename conversion_t_>
    T process_raw_type_optional_argument_option(const command_t& subcommand,
                                                const std::string& main_command_or_subcommand,
                                                const std::string& long_name, const std::optional<std::string>& short_name) {

        if (subcommand.inputs.options_to_values.contains(long_name)) {
            if (subcommand.inputs.options_to_values.at(long_name).has_value()) {
                try {
                    return conversion_t_()(*subcommand.inputs.options_to_values.at(
                            long_name)); // no need for has_value check here; returning an empty optional is valid
                }
                catch (cli_error& e) {
                    throw cli_error(
                            "Error initializing " + main_command_or_subcommand + " \"" +
                            to_string(subcommand.name) + "\" option \"" + long_name +
                            "\". Details: " + e.what(), e.error_type());
                }
            }
            else {
                return typename last::type();
            }
        }
        else if (short_name && subcommand.inputs.options_to_values.contains(*short_name)) {
            if (subcommand.inputs.options_to_values.at(*short_name).has_value()) {
                try {
                    return conversion_t_()(*subcommand.inputs.options_to_values.at(*short_name));
                }
                catch (cli_error& e) {
                    throw cli_error(
                            "Error initializing " + main_command_or_subcommand + " \"" +
                            to_string(subcommand.name) + "\" option \"" + *short_name +
                            "\" (full name \"" + long_name + "\"). Details: " +
                            e.what(), e.error_type());
                }
            }
            else {
                return typename last::type();
            }
        }
        else {
            return {}; // return empty optional
        }
    }

    template<typename T, typename last>
    T process_optional_argument_option(const command_t& subcommand,
                                                const std::string& main_command_or_subcommand,
                                                const std::string& long_name, const std::optional<std::string>& short_name) {

        if (subcommand.inputs.options_to_values.contains(long_name)) {
            try {
                return {subcommand.inputs.options_to_values.at(
                        long_name)}; // no need for has_value check here; returning an empty optional is valid
            }
            catch (cli_error& e) {
                throw cli_error("Error initializing " + main_command_or_subcommand + " \"" +
                                 to_string(subcommand.name) + "\" option \"" + long_name +
                                 "\". Details: " + e.what(), e.error_type());
            }
        }
        else if (short_name && subcommand.inputs.options_to_values.contains(*short_name)) {
            try {
                return {subcommand.inputs.options_to_values.at(
                        *short_name)}; // no need for has_value check here; returning an empty optional is valid
            }
            catch (cli_error& e) {
                throw cli_error("Error initializing " + main_command_or_subcommand + " \"" +
                                 to_string(subcommand.name) + "\" option \"" + *short_name +
                                 "\" (full name \"" + long_name + "\"). Details: " + e.what(),
                                 e.error_type());
            }
        }
        else {
            return {}; // default constructor for optional option with optional argument inits to empty optional and was_included to false
        }
    }

    template<typename T, typename last>
    T process_raw_type(unsigned cumulative_positional_index, const command_t& subcommand,
                                       const std::string& main_command_or_subcommand) {

        std::string long_name = last::name.string();

        std::optional<std::string> short_name;

        if constexpr(last::has_short_name) {
            short_name = std::string{last::short_name};
        }

        if constexpr(argument_info_t<last>::is_flag) {
            return subcommand.inputs.flags.contains(last::name.string()) ||
                   (short_name && subcommand.inputs.flags.contains(std::string{last::short_name}));
        }
        else {
            using conversion_t_ = typename last::conversion_t;

            if constexpr(argument_info_t<last>::is_option) {
                if constexpr(last::optional) { // implicitly required argument
                    if constexpr(last::argument_optional) { // I believe default_construct_when_empty is implicity true, so no need for && here
                        return process_raw_type_optional_argument_option<T, last, conversion_t_>(subcommand, main_command_or_subcommand,
                                                                                  long_name, short_name);
                    }
                    else {
                        return process_required_argument_option<T, last, conversion_t_>(subcommand, main_command_or_subcommand, 
                                                                                       long_name, short_name);
                    }
                }
                else {
                    return process_required_option<T, last, conversion_t_>(subcommand,
                                                                          main_command_or_subcommand,
                                                                          long_name, short_name);
                }
            }
            else if constexpr(argument_info_t<last>::is_positional) { // positional
                return process_positional<T, last, conversion_t_>(cumulative_positional_index, subcommand,
                                                                 main_command_or_subcommand, long_name);
            }
            else { // variadic
                return process_variadic<T, last, conversion_t_>(cumulative_positional_index, subcommand,
                                                               main_command_or_subcommand, long_name);
            }
        }
    }

    template<typename T, typename last>
    T process_wrapper(const command_t& subcommand,
                      const std::string& main_command_or_subcommand) {


        std::string long_name = T::name.string();

        std::optional<std::string> short_name;

        if constexpr(T::has_short_name) {
            short_name = std::string{T::short_name};
        }

        if constexpr(argument_info_t<T>::is_option) {
            if constexpr(T::optional && T::argument_optional) {
                return process_optional_argument_option<T, last>(subcommand,
                                                                 main_command_or_subcommand,
                                                                 long_name, short_name);
            }
            else {
                return {}; // this variable is a dummy
            }
        }
        else {
            return {}; // dummy
        }
    }

    template<typename T, typename last>
    /*add_const_ref_if_string_t<T>::*/T
    process_argument(unsigned cumulative_positional_index, const command_t& subcommand) {
        using arg_info_t = argument_info_t<T>; // no need for remove_cvref, we do that before calling process_argument

        std::string main_command_or_subcommand;

        if(subcommand.name == command_name_t{"MAIN"}) {
            main_command_or_subcommand = "main command";
        }
        else {
            main_command_or_subcommand = "subcommand";
        }

        if constexpr(arg_info_t::is_raw_type) {
            return process_raw_type<T, last>(cumulative_positional_index, subcommand,
                                                           main_command_or_subcommand);
        }
        else {
            return process_wrapper<T, last>(subcommand, main_command_or_subcommand);
        }
    }
}

//end of "process_argument.h" include

//included from file "C:\programming\hitoribooru\hitoribooru-cli\lib\cppli\single-header-amalgamation\..\include\cppli\detail\argument_processing_checks.h"

namespace cppli::detail {
    template<std::size_t current_index, std::size_t max_index, typename arg_t, typename...arg_ts>
    constexpr std::size_t count_positionals_before_index_func() {
        if constexpr(current_index < max_index) {
            return count_positionals_before_index_func<current_index+1, max_index, arg_ts...>()+argument_info_t<std::remove_cvref_t<arg_t>>::is_positional;
        }
        else {
            return 0; // return 0 instead of is_positional because inclusivity is not what we want
        }
    }

    struct dummy_t{};

    // TODO: static_assert for optional positions followed by required positionals
    template<bool optional_positional_encountered, typename arg_t, typename...arg_ts>
    constexpr bool invalid_optional_positionals_found() {
        using T = std::remove_cvref_t<arg_t>;
        constexpr bool is_positional = argument_info_t<T>::is_positional;

        if constexpr(sizeof...(arg_ts) > 0) {
            if constexpr(is_positional) {
                return (optional_positional_encountered && !T::optional) || invalid_optional_positionals_found<optional_positional_encountered || T::optional, arg_ts...>();
            }
            else {
                return invalid_optional_positionals_found<optional_positional_encountered, arg_ts...>();
            }
        }
        else {
            if constexpr(is_positional) {
                return optional_positional_encountered && !T::optional;
            }
            else {
                return false;
            }
        }
    }

    template<typename arg_t, typename...arg_ts>
    constexpr std::size_t count_variadics() {
        if constexpr(sizeof...(arg_ts) > 0) {
            return argument_info_t<arg_t>::is_variadic+count_variadics<arg_ts...>();
        }
        else {
            return 0;
        }
    }

    template<std::size_t max_index, typename...arg_ts>
    static constexpr std::size_t count_positionals_before_index_v = count_positionals_before_index_func<0, max_index, arg_ts...>();

    template<typename T, typename U, typename...Ts>
    constexpr bool pack_contains_short_name_func() {
        //static_assert(!(argument_info_t<T>::has_short_name && argument_info_t<U>::has_short_name));

        if constexpr(argument_info_t<T>::has_short_name) {
            if constexpr(argument_info_t<U>::has_short_name) {
                if constexpr(sizeof...(Ts) > 0) {
                    return (T::short_name == U::short_name) || pack_contains_short_name_func<T, Ts...>();
                }
                else {
                    return (T::short_name == U::short_name);
                }
            }
            else {
                if constexpr(sizeof...(Ts) > 0) {
                    return pack_contains_short_name_func<T, Ts...>(); //
                }
                else {
                    return false;
                }
            }
        }
        else {
            return false;
        }
    }

    template<std::size_t index, typename first_t, typename...rest_ts> // this is broken
    constexpr bool no_repeated_short_names_func() {
        if constexpr(sizeof...(rest_ts) == 0) {
            return true;
        }
        else if constexpr(index < sizeof...(rest_ts)) {
            return (!pack_contains_short_name_func<first_t, rest_ts...>()) && no_repeated_short_names_func<index+1, rest_ts..., first_t>();
        }
        else {
            return (!pack_contains_short_name_func<first_t, rest_ts...>());
        }
    }

    template<typename...arg_ts>
    constexpr bool no_repeated_short_names_v = no_repeated_short_names_func<0, std::remove_cvref_t<arg_ts>...>();


    template<typename T, typename U, typename...Ts>
    constexpr bool pack_contains_long_name_func() {
        //static_assert(!(argument_info_t<T>::has_long_name && argument_info_t<U>::has_long_name));

        if constexpr(argument_info_t<T>::has_long_name) {
            if constexpr(argument_info_t<U>::has_long_name) {
                if constexpr(sizeof...(Ts) > 0) {
                    return (T::name == U::name) || pack_contains_long_name_func<T, Ts...>();
                }
                else {
                    return (T::name == U::name);
                }
            }
            else {
                if constexpr(sizeof...(Ts) > 0) {
                    return pack_contains_long_name_func<T, Ts...>(); //
                }
                else {
                    return false;
                }
            }
        }
        else {
            return false;
        }
    }

    template<std::size_t index, typename first_t, typename...rest_ts>
    constexpr bool no_repeated_long_names_func() {
        if constexpr(sizeof...(rest_ts) == 0) {
            return true;
        }
        else if constexpr(index < sizeof...(rest_ts)) {
            return (!pack_contains_long_name_func<first_t, rest_ts...>()) && no_repeated_long_names_func<index+1, rest_ts..., first_t>();
        }
        else {
            return (!pack_contains_long_name_func<first_t, rest_ts...>());
        }
    }

    template<typename...arg_ts>
    constexpr bool no_repeated_long_names_v = no_repeated_long_names_func<0, std::remove_cvref_t<arg_ts>...>();
}
//end of "argument_processing_checks.h" include

namespace cppli::detail {
    template<typename func_t, auto func, typename index_seq>
    struct call_func_wrapper_impl_t;

    template<typename return_t, typename...arg_ts, std::size_t...indices, auto func>
    struct call_func_wrapper_impl_t<return_t(*)(const command_context_t&, arg_ts...), func, std::integer_sequence<std::size_t, indices...>> {
        static void call_func(const command_t& subcommand, const command_context_t& command_context) {
            func(command_context, process_argument<std::remove_cvref_t<get_type_from_index_in_pack<indices, arg_ts...>>, std::remove_cvref_t<get_type_from_index_in_pack<(indices)-1, arg_ts...>>>(count_positionals_before_index_v<indices, arg_ts...>, subcommand)...);
        }

        #ifdef CPPLI_FULL_ERROR_CHECKING_BEFORE_RUN
            static void check_for_errors(const command_t& subcommand) {
                // fold over comma operator
                (process_argument<std::remove_cvref_t<get_type_from_index_in_pack<indices, arg_ts...>>, std::remove_cvref_t<get_type_from_index_in_pack<(indices)-1, arg_ts...>>>(count_positionals_before_index_v<indices, arg_ts...>, subcommand), ...);
            }
        #endif
    };

    template<typename func_t, auto func>
    struct call_func_wrapper_t;

    template<typename return_t, typename...arg_ts, auto func>
    struct call_func_wrapper_t<return_t(*)(const command_context_t&, arg_ts...), func> { // we need all these ugly partial specializations so that we can deduce arg_ts and indices
        static_assert(!invalid_optional_positionals_found<false, arg_ts...>(), "Required positionals cannot follow optional positionals");

        static void call_func(const command_t& subcommand, const command_context_t& command_context) {
            constexpr auto variadic_count = count_variadics<std::remove_cvref_t<arg_ts>...>();
            static_assert(variadic_count < 2, "A command can only have 0 or 1 variadics (two or more were found)");

            call_func_wrapper_impl_t<decltype(func), func, std::make_index_sequence<sizeof...(arg_ts)>>::call_func(subcommand, command_context);
        }

        #ifdef CPPLI_FULL_ERROR_CHECKING_BEFORE_RUN
            static void check_for_errors(const command_t& subcommand) {
                call_func_wrapper_impl_t<decltype(func), func, std::make_index_sequence<sizeof...(arg_ts)>>::check_for_errors(subcommand);
            }
        #endif
    };

    template<auto func>
    struct call_func_wrapper_t<void(*)(const command_context_t&), func> { // we need all this ugly partial specializations so that we can deduce arg_ts and indices
        static void call_func(const command_t& subcommand, const command_context_t& command_context) {
            func(command_context);
        }

        static void check_for_errors(const command_t& subcommand) {
            // do nothing
        }
    };

    template<auto func>
    void call_func(const command_t& command, const command_context_t& command_context) {
        call_func_wrapper_t<decltype(func), func>::call_func(command, command_context);
    }

    #ifdef CPPLI_FULL_ERROR_CHECKING_BEFORE_RUN
        template<auto func>
        void check_for_errors(const command_t& command) {
            call_func_wrapper_t<decltype(func), func>::check_for_errors(command);
        }
    #endif

    template<typename return_t, typename arg_t, typename...arg_ts>                      // don't actually care about func, just need to deduce args
    void generate_input_info_and_docs_impl(command_inputs_info_t& info, command_documentation_t& documentation, return_t(*func)(const command_context_t&, arg_t, arg_ts...) = nullptr) {
        using T = std::remove_cvref_t<arg_t>;
        using arg_info_t = argument_info_t<T>;

        if constexpr(!arg_info_t::is_raw_type) {
            using type = std::remove_cvref_t<arg_t>;

            static_assert(no_repeated_long_names_v<arg_t, arg_ts...>,  "multiple flags/options cannot share a long name"); // I don't think this is necessary anymore

            if constexpr(arg_info_t::is_flag) {
                documentation.flags.emplace(type::name.string(),
                                            type::documentation.string(),
                                            type::short_name);

                info.flags.insert(type::name.string());

                if constexpr(type::short_name != '\0') {
                    info.flags.insert(std::string{type::short_name});

                    info.flag_or_option_short_name_to_long_name.emplace(T::short_name, T::name.string());
                    info.flag_or_option_long_name_to_short_name.emplace(T::name.string(),  std::string{T::short_name});
                }
            }
            else if constexpr(arg_info_t::is_option) {
                documentation.options.emplace(type::type_string.string(),
                                              type::name.string(),
                                              type::argument_text.string(),
                                              type::documentation.string(),
                                              type::short_name,

                                              type::optional,
                                              type::argument_optional);

                info.option_argument_is_optional.emplace(type::name.string(), type::argument_optional);

                if constexpr(type::short_name != '\0') {
                    info.option_argument_is_optional.emplace(std::string{type::short_name}, type::argument_optional);

                    info.flag_or_option_short_name_to_long_name.emplace(T::short_name, T::name.string());
                    info.flag_or_option_long_name_to_short_name.emplace(T::name.string(),  std::string{T::short_name});
                }
            }
            else if constexpr(arg_info_t::is_positional) { // positional
                documentation.positionals.emplace_back(type::type_string.string(),
                                                       type::name.string(),
                                                       type::documentation.string(),

                                                       type::optional);
            }
            else { // variadic
                documentation.variadic = variadic_documentation_t{type::type_string.string(),
                                          type::name.string(),
                                          type::documentation.string()};
            }
        }

        if constexpr(sizeof...(arg_ts) > 0) {
            generate_input_info_and_docs_impl<return_t, arg_ts...>(info, documentation);
        }
    }

    template<typename return_t, typename arg_t, typename...arg_ts>                      // don't actually care about func, just need to deduce args
    void generate_input_info_and_docs(command_inputs_info_t& info, command_documentation_t& documentation, return_t(*func)(const command_context_t&, arg_t, arg_ts...) = nullptr) {
        static_assert(no_repeated_short_names_v<arg_t, arg_ts...>, "multiple parameters cannot have the same short name");
        generate_input_info_and_docs_impl(info, documentation, func);
    }

    template<typename return_t>                 // don't actually care about func, just need to deduce args
    void generate_input_info_and_docs(command_inputs_info_t& info, command_documentation_t& documentation, return_t(*func)(const command_context_t&) = nullptr) {
        // do nothing
    }

    // we don't have command_macros yet, so we have to write the callback signature manually
    void default_help_callback(const command_context_t& cppli_current_command,

                               const variadic<std::string, string_conversion_t<std::string>, false, "subcommand name", "The name of the subcommand to print help for."
                                                                                                                "If no subcommand is provided, then help is printed for the parent command">&, std::vector<std::string> subcommand_name,

                               const flag<"name-only", "only print subcommand names">&, bool name_only,
                               const flag<"name-and-description", "print subcommand name and description">&, bool name_and_description,
                               const flag<"name-and-args", "print subcommand name and args">&, bool name_and_args,
                               const flag<"name-description-and-args", "print subcommand name, description, and args">&, bool name_description_and_args,
                               const flag<"verbose", "print current command name and description", 'v'>&, bool verbose,
                               const flag<"hide-help", "don't show help when printing subcommands">&, bool hide_help,
                               const flag<"show-help", "do show help when printing subcommands">&, bool show_help,

                               const flag<"subcommands-name-only", "only print subcommand names">&, bool subcommands_name_only,
                               const flag<"subcommands-name-and-description", "print subcommand name and description">&, bool subcommands_name_and_description,
                               const flag<"subcommands-name-and-args", "print subcommand name and args">&, bool subcommands_name_and_args,
                               const flag<"subcommands-name-description-and-args", "print subcommand name, description, and args">&, bool subcommands_name_description_and_args,
                               const flag<"subcommands-verbose", "print subcommand name and description">&, bool subcommands_verbose,

                               const option<unsigned, string_conversion_t<unsigned>, false, "recursion", "how many levels of nested subcommands to print. 0 prints none", "unsigned integer", true, false, 'r'>&, const std::optional<unsigned>& recursion);

    template<auto func>
    dummy_t register_command(const command_name_t& name, const char* description, bool is_help = false) {
        command_inputs_info_t   info;
        command_documentation_t docs(name.back(), description);

        command_name_t cumulative_name;
        for(unsigned i = 0; i < name.size()-1; ++i) {
            cumulative_name.push_back(name[i]);
            if(!subcommand_name_to_docs().contains(cumulative_name)) {
                subcommand_name_to_docs()[cumulative_name].name = cumulative_name.back();
            }
            subcommand_name_to_docs()[cumulative_name].subcommands.emplace(name[i+1]);
        }

        /*if(name.front() != "MAIN") {
            subcommand_name_to_docs()[{"MAIN"}].subcommands.emplace(name.front());
        }*/

        generate_input_info_and_docs(info, docs, func);

        subcommand_name_to_func().insert_or_assign(name, call_func<func>);
        #ifdef CPPLI_FULL_ERROR_CHECKING_BEFORE_RUN
            subcommand_name_to_error_checking_func().insert_or_assign(name, check_for_errors<func>);
        #endif
        subcommand_name_to_inputs_info().insert_or_assign(name, std::move(info));

        if(name == command_name_t{"MAIN"}) {
            subcommand_name_to_docs()[name].flags       = std::move(docs.flags);
            subcommand_name_to_docs()[name].options     = std::move(docs.options);
            subcommand_name_to_docs()[name].positionals = std::move(docs.positionals);
            subcommand_name_to_docs()[name].variadic    = std::move(docs.variadic);

            subcommand_name_to_docs()[name].is_namespace = false;

            // can't do a full assignment because that would wipe subcommands
        }
        else {
            subcommand_name_to_docs().insert_or_assign(name, std::move(docs));
        }

        if(!is_help) {
            command_name_t temp;
            for(const auto& command : name) {
                temp.push_back(command);

                command_name_t temp_plus_help = temp;
                temp_plus_help.push_back("help");
                if(!subcommand_name_to_func().contains(temp_plus_help)) {
                    register_command<default_help_callback>(temp_plus_help, "print help for this command", true);
                }
            }
        }

        return {};
    }
}

//end of "detail/command_registration.h" include

#ifdef _MSC_VER
    #ifndef _MSVC_TRADITIONAL
        #error "CPPLI won't work on this version of MSVC, sorry! The version of MSVC you're using doesn't support the new preprocessor (/Zc:preprocessor)"
    #elif _MSVC_TRADITIONAL == 1
        #error "You're using the traditional preprocessor, which won't work with CPPLI. Recompile with /Zc:preprocessor"
    #endif
#endif

namespace cppli {
    bool current_command_is_leaf();

    //#define CPPLI_NAMESPACE(NAME, DESCRIPTION)

    #define CPPLI_MAIN_COMMAND(/*parameters*/...) \
    /*extern "C"*/ void cPPLI_internal_GENERATED_MAIN_COMMAND_CALLBACK (const ::cppli::command_context_t& __VA_OPT__(, __VA_ARGS__)); \
    cPPLI_internal_EVALUATE_AT_FILE_SCOPE(::cppli::detail::register_command<cPPLI_internal_GENERATED_MAIN_COMMAND_CALLBACK>({"MAIN"}, "")) \
    /*extern "C"*/ void cPPLI_internal_GENERATED_MAIN_COMMAND_CALLBACK (const ::cppli::command_context_t& cppli __VA_OPT__(, __VA_ARGS__))

    #define CPPLI_SUBCOMMAND(name, DESCRIPTION, /*parameters*/...) \
    /*extern "C"*/ void cPPLI_internal_CAT(cPPLI_internal_GENERATED_SUBCOMMAND_CALLBACK, name) (const ::cppli::command_context_t& __VA_OPT__(, __VA_ARGS__)); \
    static_assert(!::cppli::detail::contains_uppercase<cPPLI_internal_STRINGIFY(cPPLI_internal_CAT(name))>(), "subcommand names cannot contain uppercase characters"); \
    cPPLI_internal_EVALUATE_AT_FILE_SCOPE(::cppli::detail::register_command<cPPLI_internal_CAT(cPPLI_internal_GENERATED_SUBCOMMAND_CALLBACK, name)>({cPPLI_internal_FOR_EACH(cPPLI_internal_STRINGIFY_WITH_COMMA, MAIN, name)}, DESCRIPTION)) \
    /*extern "C"*/ void cPPLI_internal_CAT(cPPLI_internal_GENERATED_SUBCOMMAND_CALLBACK, name) (const ::cppli::command_context_t& cppli __VA_OPT__(, __VA_ARGS__))

    #define CPPLI_NAME(...) __VA_ARGS__

    /// the optional last argument is a single character short name
    #define CPPLI_FLAG(NAME, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::flag<cPPLI_internal_STRINGIFY(NAME), DESCRIPTION __VA_OPT__(, cPPLI_internal_STRINGIFY(__VA_ARGS__)[0])>&, bool NAME

    /// the optional last argument is a single character short name
    #define CPPLI_OPTION(TYPE, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, ::cppli::string_conversion_t<std::optional<TYPE>>, false, cPPLI_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, false __VA_OPT__(, cPPLI_internal_STRINGIFY(__VA_ARGS__)[0])>&, const std::optional<TYPE>& NAME

    /// the optional last argument is a single character short name
    #define CPPLI_OPTION_CONVERSION(TYPE, CONVERSION_T, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, CONVERSION_T, false, cPPLI_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, false __VA_OPT__(, cPPLI_internal_STRINGIFY(__VA_ARGS__)[0])>&, const std::optional<TYPE>& NAME

    /// same as CPPLI_OPTION, but the raw TYPE is used (without std::optional)
    /// If the option is not provided, a default constructed object is passed to the callback instead of an empty optional
    /// The optional last argument is a single character short name
    #define CPPLI_OPTION_DEFAULT_CTOR(TYPE, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, ::cppli::string_conversion_t<TYPE>, true, cPPLI_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, false __VA_OPT__(, cPPLI_internal_STRINGIFY(__VA_ARGS__)[0])>&, const TYPE& NAME

    #define CPPLI_OPTION_CONVERSION_DEFAULT_CTOR(TYPE, CONVERSION_T, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, CONVERSION_T, true, cPPLI_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, false __VA_OPT__(, cPPLI_internal_STRINGIFY(__VA_ARGS__)[0])>&, const TYPE& NAME


    /// the optional last argument is a single character short name
    #define CPPLI_OPTIONAL_ARGUMENT_OPTION(TYPE, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, ::cppli::string_conversion_t<std::optional<TYPE>>, false, cPPLI_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, true __VA_OPT__(, cPPLI_internal_STRINGIFY(__VA_ARGS__)[0])>& NAME

    /// the optional last argument is a single character short name
    #define CPPLI_OPTIONAL_ARGUMENT_OPTION_CONVERSION(TYPE, CONVERSION_T, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, CONVERSION_T, false, cPPLI_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, true __VA_OPT__(, cPPLI_internal_STRINGIFY(__VA_ARGS__)[0])>& NAME

    /// the optional last argument is a single character short name
    #define CPPLI_OPTIONAL_ARGUMENT_OPTION_DEFAULT_CTOR(TYPE, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, ::cppli::string_conversion_t<std::optional<TYPE>>, false, cPPLI_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, true __VA_OPT__(, cPPLI_internal_STRINGIFY(__VA_ARGS__)[0])>&, const std::optional<TYPE>& NAME

    /// the optional last argument is a single character short name
    #define CPPLI_OPTIONAL_ARGUMENT_OPTION_CONVERSION_DEFAULT_CTOR(TYPE, CONVERSION_T, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, CONVERSION_T, false, cPPLI_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, true __VA_OPT__(, cPPLI_internal_STRINGIFY(__VA_ARGS__)[0])>&, const std::optional<TYPE>& NAME



    /// the optional last argument is a single character short name
    #define CPPLI_REQUIRED_OPTION(TYPE, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, ::cppli::string_conversion_t<TYPE>, false, cPPLI_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, false, false __VA_OPT__(, cPPLI_internal_STRINGIFY(__VA_ARGS__)[0])>&, const TYPE& NAME

    /// the optional last argument is a single character short name
    #define CPPLI_REQUIRED_OPTION_CONVERSION(TYPE, CONVERSION_T, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, CONVERSION_T, false, cPPLI_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, false, false __VA_OPT__(, cPPLI_internal_STRINGIFY(__VA_ARGS__)[0])>&, const TYPE& NAME


    #define CPPLI_POSITIONAL(TYPE, NAME, DESCRIPTION) \
    const ::cppli::detail::positional<TYPE, ::cppli::string_conversion_t<TYPE>, false, false, cPPLI_internal_STRINGIFY(NAME), DESCRIPTION>&, const TYPE& NAME

    #define CPPLI_POSITIONAL_CONVERSION(TYPE, CONVERSION_T, NAME, DESCRIPTION) \
    const ::cppli::detail::positional<TYPE, CONVERSION_T, false, false, cPPLI_internal_STRINGIFY(NAME), DESCRIPTION>&, const TYPE& NAME


    #define CPPLI_OPTIONAL_POSITIONAL(TYPE, NAME, DESCRIPTION) \
    const ::cppli::detail::positional<TYPE, ::cppli::string_conversion_t<std::optional<TYPE>>, false, true, cPPLI_internal_STRINGIFY(NAME), DESCRIPTION>&, const std::optional<TYPE>& NAME

    #define CPPLI_OPTIONAL_POSITIONAL_CONVERSION(TYPE, CONVERSION_T, NAME, DESCRIPTION) \
    const ::cppli::detail::positional<TYPE, CONVERSION_T, false, true, cPPLI_internal_STRINGIFY(NAME), DESCRIPTION>&, const std::optional<TYPE>& NAME

    #define CPPLI_OPTIONAL_POSITIONAL_DEFAULT_CTOR(TYPE, NAME, DESCRIPTION) \
    const ::cppli::detail::positional<TYPE, ::cppli::string_conversion_t<TYPE>, false, true, cPPLI_internal_STRINGIFY(NAME), DESCRIPTION>&, const TYPE& NAME

    #define CPPLI_OPTIONAL_POSITIONAL_CONVERSION_DEFAULT_CTOR(TYPE, CONVERSION_T, NAME, DESCRIPTION) \
    const ::cppli::detail::positional<TYPE, CONVERSION_T, false, true, cPPLI_internal_STRINGIFY(NAME), DESCRIPTION>&, const TYPE& NAME


    #define CPPLI_VARIADIC(TYPE, NAME, DESCRIPTION) \
    const ::cppli::detail::variadic<TYPE, ::cppli::string_conversion_t<TYPE>, false, cPPLI_internal_STRINGIFY(NAME), DESCRIPTION>&, const std::vector<TYPE>& NAME

    #define CPPLI_VARIADIC_CONVERSION(TYPE, CONVERSION_T, NAME, DESCRIPTION) \
    const ::cppli::detail::variadic<TYPE, CONVERSION_T, false, cPPLI_internal_STRINGIFY(NAME), DESCRIPTION>&, const std::vector<TYPE>& NAME
}
//end of "command_macros.h" include

//included from file "../include/cppli\configuration.h"


//end of "configuration.h" include

//included from file "../include/cppli\documentation.h"



//end of "documentation.h" include

//included from file "../include/cppli\run.h"


namespace cppli {
    namespace detail {
        int run_impl_(const std::vector<std::string>& arg_vec);
        int run_impl_(int argc, const char* const* argv);

        template<string_literal program_name, string_literal description> // TODO: put run in its own file
        void set_program_name_and_description_from_template_args() {
            static_assert(detail::all_lowercase_numeral_or_hyphen<program_name>(), "command names can only contain lowercase characters, numerals, and hyphens");

            detail::set_program_name_and_description(program_name.string(), description.string());

            detail::subcommand_name_to_docs()[{"MAIN"}].name = program_name;
            detail::subcommand_name_to_docs()[{"MAIN"}].description = description;
        }
    }

    template<string_literal program_name, string_literal description>
    int run(const std::vector<std::string>& arg_vec) {
        detail::set_program_name_and_description_from_template_args<program_name, description>();

        return detail::run_impl_(arg_vec);
    }

    template<string_literal program_name, string_literal description>
    int run(int argc, const char* const* argv) {
        detail::set_program_name_and_description_from_template_args<program_name, description>();

        return detail::run_impl_(argc, argv);
    }

    #ifdef _WIN32
        std::vector<std::string> wmain_utf16_argv_to_utf8(int argc, wchar_t** argv);

        #define cPPLI_internal_CONVERT_UTF16_IF_WINDOWS(ARGC, ARGV) cppli::wmain_utf16_argv_to_utf8(ARGC, ARGV)
        #define cPPLI_internal_MAIN_FUNCTION_HEADER int wmain(int argc, wchar_t *argv[])
    #else
        #define cPPLI_internal_CONVERT_UTF16_IF_WINDOWS(ARGC, ARGV) ARGC, ARGV
        #define cPPLI_internal_MAIN_FUNCTION_HEADER(ARGC, ARGV) int main(int argc, char** argv)
    #endif
}

#define CPPLI_DEFINE_MAIN_FUNCTION(PROGRAM_NAME, PROGRAM_DESCRIPTION)                                                  \
    cPPLI_internal_MAIN_FUNCTION_HEADER {                                                                              \
            return cppli::run<PROGRAM_NAME, PROGRAM_DESCRIPTION>(cPPLI_internal_CONVERT_UTF16_IF_WINDOWS(argc, argv)); \
        }


//end of "run.h" include

#ifdef CPPLI_IMPLEMENTATION
    #define IRO_IMPL // necessary because if an include that isn't preceded by #define *_IMPL is expanded before the include that *is* preceded by it,
    #define ORI_IMPL // the automatic header guard will kick in and prevent the file from being expanded again, preventing the implementation from being pasted in, which will cause undefined reference errors

//included from file "../src/configuration.cpp"

#include <iostream>


//included from file "../lib/iro\iro.h"

#include <unordered_map>
#include <array>
#include <ostream>
#include <sstream>
#include <vector>
#include <cassert>

#if defined(__unix__) || defined(__unix) || defined(__APPLE__) || defined(__MACH__)
    #define IRO_UNIX
    #include <unistd.h>
#elif defined(_WIN32)
    #define IRO_WINDOWS
    #include <cstdio>
    #include <io.h>
#endif

namespace iro {
    enum effect_type {
        foreground_color,
        background_color,
        font_weight,
        underlinedness, // I know this isn't a real word
        blink,

        number_of_effect_types
    };

    namespace color {
        enum color {
            black,
            red,
            green,
            yellow,
            blue,
            magenta,
            cyan,
            white,

            bright_black,
            bright_red,
            bright_green,
            bright_yellow,
            bright_blue,
            bright_magenta,
            bright_cyan,
            bright_white
        };
    }


    class effect;
    class effect_set;

    namespace detail {
        effect create(const char* code, effect_type type) noexcept;
    }

    class effect {
        const char* code_;
        effect_type type_;

        effect(const char* code, effect_type type) noexcept;

        friend effect detail::create(const char* code, effect_type type) noexcept; // Make this private so that the user can't construct invalid effects
        friend class effect_set;
        friend class persist;

        friend effect_set   operator|(const effect& e, const effect_set& es);
        friend effect_set&& operator|(const effect& e, effect_set&& es);
    };

    ///  effects  ///
        ///  foreground colors  ///
            extern const effect black          ;
            extern const effect red            ;
            extern const effect green          ;
            extern const effect yellow         ;
            extern const effect blue           ;
            extern const effect magenta        ;
            extern const effect cyan           ;
            extern const effect white          ;

            extern const effect bright_black   ;
                extern const effect& gray ;
                extern const effect& grey ;
            extern const effect bright_red     ;
            extern const effect bright_green   ;
            extern const effect bright_yellow  ;
            extern const effect bright_blue    ;
            extern const effect bright_magenta ;
            extern const effect bright_cyan    ;
            extern const effect bright_white   ;
        /// /foreground colors  ///

        ///  background colors  ///
            extern const effect background_black          ;
            extern const effect background_red            ;
            extern const effect background_green          ;
            extern const effect background_yellow         ;
            extern const effect background_blue           ;
            extern const effect background_magenta        ;
            extern const effect background_cyan           ;
            extern const effect background_white          ;

            extern const effect background_bright_black   ;
                extern const effect& background_gray ;
                extern const effect& background_grey ;
            extern const effect background_bright_red     ;
            extern const effect background_bright_green   ;
            extern const effect background_bright_yellow  ;
            extern const effect background_bright_blue    ;
            extern const effect background_bright_magenta ;
            extern const effect background_bright_cyan    ;
            extern const effect background_bright_white   ;
        /// /background colors  ///

        ///  font weight  ///
            extern const effect bold          ;
            extern const effect faint         ;
            extern const effect normal_weight ;
        /// /font weight  ///

        /// underline  ///
            extern const effect underlined     ;
                extern const effect& underline ; // I can't decide whether to call this one underlined or underlineD, so I'll just let both be valid
            extern const effect not_underlined ;
        /// /underline  ///

        ///  blink  ///
            extern const effect blinking     ;
            extern const effect not_blinking ;
        /// /blink  ///

    /// /effects  ///

    class effect_set;
    namespace detail{
        unsigned push_persist(std::ostream* stream, const effect_set& effects);
        void set(std::ostream* stream, unsigned index, const effect_set& effects);
        unsigned copy_persist(std::ostream* stream, unsigned index_in_stack);
    }
    class effect_set {
        std::array<const char*, number_of_effect_types> type_to_code_;

        friend class persist;
        friend class effect_string;

        friend effect_set   operator|(const effect& e, const effect_set& es);
        friend effect_set&& operator|(const effect& e, effect_set&& es);

        friend unsigned detail::push_persist(std::ostream* stream, const effect_set& effects);
        friend void detail::set(std::ostream* stream, unsigned index, const effect_set& effects);

        friend unsigned detail::copy_persist(std::ostream* stream, unsigned index_in_stack);

        effect_set(const std::array<const char*, number_of_effect_types>& type_to_code);

    public:
        effect_set();

        effect_set(const effect& e);
        effect_set(const effect& e, const effect& e2);

        effect_set   operator| (const effect& rhs) const&;
        effect_set&& operator| (const effect& rhs)&&;
        effect_set&  operator|=(const effect& rhs)&;
        effect_set&& operator|=(const effect& rhs)&&;

        effect_set   operator| (const effect_set& rhs) const&;
        effect_set&& operator| (const effect_set& rhs)&&;
        effect_set&  operator|=(const effect_set& rhs)&;
        effect_set&& operator|=(const effect_set& rhs)&&;
    };

    effect_set   operator|(const effect& e1, const effect& e2);
    effect_set   operator|(const effect& e, const effect_set& es);
    effect_set&& operator|(const effect& e, effect_set&& es);


    namespace detail {
        unsigned push_persist(std::ostream* stream, effect_type type, const char* code);
        unsigned push_empty_persist(std::ostream* stream); // TODO: change all instances of effect in function names to persist, because now all effects are bundled into one entry in the stack
        void pop_effect(std::ostream* stream);
        void delete_persist(std::ostream* stream, unsigned index_in_stack);
        void set(std::ostream* stream, unsigned index, effect_type type, const char* code);
        void set_top(std::ostream* stream, effect_type type, const char* code);
        const char* get_top_code(const std::ostream* stream, effect_type type);
        void reapply_top(std::ostream* stream, effect_type type);
        bool persist_has_effect_of_type(std::ostream* stream, unsigned index, effect_type type);

        template<typename T>
        struct filled_array_helper_t {
            const T& val;

            filled_array_helper_t(const T& val) : val(val) {}

            template<std::size_t N>
            operator std::array<T,N>() {
                std::array<T,N> ret;
                ret.fill(val);
                return ret;
            }
        };

        template<typename T>
        filled_array_helper_t<T> filled_array(const T& value) {
            return filled_array_helper_t<T>(value);
        }

        template<std::size_t N, typename T>
        std::array<T,N> filled_array(const T& value) {
            std::array<T,N> ret;
            ret.fill(value);
            return ret;
        }

        template<std::size_t N, typename T, typename U>
        std::array<T,N> fill_array_and_set_one_value(const T& fill_value, unsigned index, const U& set_value) {
            auto arr = filled_array<T, N>(fill_value);
            arr[index] = set_value;
            return arr;
        }
    }


    class effect_string;

    class persist {
        unsigned location_in_stack_;            // 0 is a special value that indicates a persist with no active effects
        std::ostream* stream_ = nullptr;        // 0 is always available because index 0 (the first element in the stack) is always the default effect set

        static constexpr unsigned empty_persist_location = 0;

        friend persist   operator<<(std::ostream&, const effect_string&);
        friend persist&& operator<<(persist&,      const effect_string&);

    public:
        persist() = delete;

        persist(std::ostream& os);

        persist(std::ostream& os, const effect& e);
        persist(std::ostream& os, const effect_set& e);

        persist(persist&& other) noexcept;
        persist& operator=(persist&& rhs) noexcept;

        persist(const persist& other);
        persist& operator=(const persist& rhs);

        persist&& operator<<(const effect& e);
        persist&& operator<<(const effect_set& es);

        template<typename T, std::enable_if_t<(!std::is_same<T, effect>::value) && (!std::is_same<T, effect_set>::value), bool> = true>
        persist&& operator<<(const T& arg) {
            *stream_ << arg;
            return std::move(*this);
        }

        std::ostream& ostream();
        const std::ostream& ostream() const;

        operator std::ostream&();
        operator const std::ostream&() const;

        void delete_early();

        ~persist();
    };

    persist operator<<(std::ostream& stream, const effect& e);
    persist operator<<(std::ostream& stream, const effect_set& e);


    class effect_string {
        struct string_and_effects {
            std::string string;
            std::array<bool, number_of_effect_types> active_effects;

            inline string_and_effects() {
                active_effects.fill(false);
            }
        };
        std::vector<string_and_effects> strings_;

        template<typename T>
        void init_(std::stringstream& stream, const T& arg) {
            stream << arg;
        }

        template<typename T, typename...Ts, std::enable_if_t<(sizeof...(Ts) > 0), bool> = true>
        void init_(std::stringstream& stream, const T& arg, const Ts&...args) {
            stream << arg;
            init_(stream, args...);
        }

        friend persist   operator<<(std::ostream&, const effect_string&);
        friend persist&& operator<<(persist&,      const effect_string&);

        string_and_effects& back_();
        const string_and_effects& back_() const;

    public:
        template<typename T, typename...Ts>
        effect_string(const effect_set& effects, const T& arg, const Ts&...args) {
            std::stringstream stream; // probably really slow
            for(unsigned i = 0; i < effects.type_to_code_.size(); ++i) {
                const auto& e = effects.type_to_code_[i];
                if(e) {
                    strings_.resize(1);
                    stream << e;
                    strings_.back().active_effects[i] = true;
                }
            }
            init_(stream, arg, args...);
            strings_.back().string = stream.str();
        }

        template<typename T, std::enable_if_t<(!std::is_same<T, effect_string>::value), bool> = true>
        effect_string& operator<<(T& arg) {
            back_() += arg;
            return *this;
        }
        effect_string& operator<<(const effect_string& arg);
        effect_string& operator<<(effect_string&& arg);

        template<typename T, std::enable_if_t<(!std::is_same<T, effect_string>::value), bool> = true>
        effect_string& operator+=(T&& arg) {

            std::stringstream sstream;
            sstream << std::forward<T>(arg);

            if(back_().active_effects == detail::filled_array<number_of_effect_types>(false)) {
                back_().string += sstream.str();
            }
            else {
                string_and_effects se;
                se.string = sstream.str();

                strings_.push_back(std::move(se));
            }

            return *this;
        }

        effect_string& operator+=(const effect_string& arg);
        effect_string& operator+=(effect_string&& arg);

        template<typename T>
        effect_string operator+(T&& arg) const& {
            auto ret = *this;
            ret += std::forward<T>(arg);
            return ret;
        }

        template<typename T>
        effect_string&& operator+(T&& arg)&& {
            *this += std::forward<T>(arg);
            return std::move(*this);
        }

        /**
         * Returns an std::string with the correct escape codes embedded
         *
         * This function is unsafe because the conversion to an std::string embeds escape codes directly as character data.
         * This means that the escape codes to undo the effects are
         *
         * You need to make absolutely sure that you don't modify any iro state between the call to unsafe_string and the resultant string being printed to the given stream
         * And (obviously), don't print the returned string to a stream othere than the one you said you were going to print it to
         *
         * You should really only use this function if you need to interface with a different printing library
         *
         * @param stream the stream you will eventually print this string to
         */
        std::string unsafe_string(const std::ostream& stream) const;
    };

    persist   operator<<(std::ostream& os, const effect_string& es);
    persist&& operator<<(persist& p,       const effect_string& es);

    #ifdef IRO_IMPL
        effect::effect(const char* code, effect_type type) noexcept : code_(code), type_(type) {}
        
        ///  effects  ///
            ///  foreground colors  ///
                const effect black          = detail::create("\x1b[30m", foreground_color);
                const effect red            = detail::create("\x1b[31m", foreground_color);
                const effect green          = detail::create("\x1b[32m", foreground_color);
                const effect yellow         = detail::create("\x1b[33m", foreground_color);
                const effect blue           = detail::create("\x1b[34m", foreground_color);
                const effect magenta        = detail::create("\x1b[35m", foreground_color);
                const effect cyan           = detail::create("\x1b[36m", foreground_color);
                const effect white          = detail::create("\x1b[37m", foreground_color);
    
                const effect bright_black   = detail::create("\x1b[90m", foreground_color);
                    const effect& gray = bright_black;
                    const effect& grey = bright_black;
                const effect bright_red     = detail::create("\x1b[91m", foreground_color);
                const effect bright_green   = detail::create("\x1b[92m", foreground_color);
                const effect bright_yellow  = detail::create("\x1b[93m", foreground_color);
                const effect bright_blue    = detail::create("\x1b[94m", foreground_color);
                const effect bright_magenta = detail::create("\x1b[95m", foreground_color);
                const effect bright_cyan    = detail::create("\x1b[96m", foreground_color);
                const effect bright_white   = detail::create("\x1b[97m", foreground_color);
            /// /foreground colors  ///
    
            ///  background colors  ///
                const effect background_black          = detail::create("\x1b[40m",  background_color);
                const effect background_red            = detail::create("\x1b[41m",  background_color);
                const effect background_green          = detail::create("\x1b[42m",  background_color);
                const effect background_yellow         = detail::create("\x1b[43m",  background_color);
                const effect background_blue           = detail::create("\x1b[44m",  background_color);
                const effect background_magenta        = detail::create("\x1b[45m",  background_color);
                const effect background_cyan           = detail::create("\x1b[46m",  background_color);
                const effect background_white          = detail::create("\x1b[47m",  background_color);
    
                const effect background_bright_black   = detail::create("\x1b[100m", background_color);
                    const effect& background_gray = bright_black;
                    const effect& background_grey = bright_black;
                const effect background_bright_red     = detail::create("\x1b[101m", background_color);
                const effect background_bright_green   = detail::create("\x1b[102m", background_color);
                const effect background_bright_yellow  = detail::create("\x1b[103m", background_color);
                const effect background_bright_blue    = detail::create("\x1b[104m", background_color);
                const effect background_bright_magenta = detail::create("\x1b[105m", background_color);
                const effect background_bright_cyan    = detail::create("\x1b[106m", background_color);
                const effect background_bright_white   = detail::create("\x1b[107m", background_color);
            /// /background colors  ///
    
            ///  font weight  ///
                const effect bold          = detail::create("\x1b[1m",  font_weight);
                const effect faint         = detail::create("\x1b[2m",  font_weight);
                const effect normal_weight = detail::create("\x1b[22m", font_weight);
            /// /font weight  ///
    
            /// underline  ///
                const effect underlined     = detail::create("\x1b[4m",  underlinedness);
                    const effect& underline = underlined; // I can't decide whether to call this one underlined or underlineD, so I'll just let both be valid
                const effect not_underlined = detail::create("\x1b[24m", underlinedness);
            /// /underline  ///
    
            ///  blink  ///
                const effect blinking     = detail::create("\x1b[5m",  blink);
                const effect not_blinking = detail::create("\x1b[25m", blink);
            /// /blink  ///
    
        /// /effects  ///

        effect_set::effect_set(const std::array<const char*, number_of_effect_types>& type_to_code) : type_to_code_(type_to_code) {}

        effect_set::effect_set() {
            type_to_code_.fill(nullptr);
        }

        effect_set::effect_set(const effect& e) {
            type_to_code_.fill(nullptr);
            type_to_code_[e.type_] = e.code_;
        }

        effect_set::effect_set(const effect& e1, const effect& e2) {
            type_to_code_.fill(nullptr);
            type_to_code_[e1.type_] = e1.code_;
            type_to_code_[e2.type_] = e2.code_;
        }

        effect_set effect_set::operator|(const effect& rhs) const& {
            effect_set ret = *this;
            ret.type_to_code_[rhs.type_] = rhs.code_;

            return ret;
        }

        effect_set&& effect_set::operator|(const effect& rhs)&& {
            return std::move(*this |= rhs);
        }

        effect_set& effect_set::operator|=(const effect& rhs)& {
            type_to_code_[rhs.type_] = rhs.code_;

            return *this;
        }

        effect_set&& effect_set::operator|=(const effect& rhs)&& {
            return std::move(*this|=rhs);
        }

        effect_set effect_set::operator|(const effect_set& rhs) const& {
            effect_set ret;
            ret |= rhs;
            return ret;
        }

        effect_set&& effect_set::operator|(const effect_set& rhs)&& {
            return std::move(*this |= rhs); // avoids creating unnecessary temporaries
        }

        effect_set& effect_set::operator|=(const effect_set& rhs)& {
            for(unsigned i = 0; i < rhs.type_to_code_.size(); ++i) {
                const auto& e = rhs.type_to_code_[i];
                if(e) {
                    type_to_code_[i] = e;
                }
            }

            return *this;
        }

        effect_set&& effect_set::operator|=(const effect_set& rhs)&& {
            return std::move(*this |= rhs);
        }

        effect_set operator|(const effect& e1, const effect& e2) {
            return {e1, e2};
        }

        effect_set operator|(const effect& e, const effect_set& es) {
            if(es.type_to_code_[e.type_]) {
                return std::move(es);
            }
            else {
                return es|e;
            }
        }

        effect_set&& operator|(const effect& e, effect_set&& es) { // FIXME:
            if(es.type_to_code_[e.type_]) {
                return std::move(es);
            }
            else {
                return std::move(es |= e);
            }
        }


        persist::persist(std::ostream& os) : stream_(&os) {
            location_in_stack_ = detail::push_empty_persist(stream_);
        }

        persist::persist(std::ostream& os, const effect& e) : stream_(&os) {
            location_in_stack_ = detail::push_persist(stream_, e.type_, e.code_);
        }

        persist::persist(std::ostream& os, const effect_set& e) : stream_(&os) {
            location_in_stack_ = detail::push_persist(stream_, e);
        }

        persist::persist(persist&& other) noexcept {
            stream_ = other.stream_;
            other.stream_ = nullptr;

            location_in_stack_ = other.location_in_stack_;
            other.location_in_stack_ = 0;
        }

        persist& persist::operator=(persist&& rhs) noexcept {
            std::swap(stream_, rhs.stream_);
            std::swap(location_in_stack_, rhs.location_in_stack_);

            return *this;
        }

        persist::persist(const persist& other) : stream_(other.stream_),
                                                 location_in_stack_(detail::copy_persist(other.stream_, other.location_in_stack_)) {}

        persist& persist::operator=(const persist& rhs) {
            return (*this = persist(rhs));
        }

        persist&& persist::operator<<(const effect& e) {
            detail::set(stream_, location_in_stack_, e.type_, e.code_);

            return std::move(*this);
        }

        persist&& persist::operator<<(const effect_set& es) {
            detail::set(stream_, location_in_stack_, es);

            return std::move(*this);
        }

        persist operator<<(std::ostream& stream, const effect& e) {
            return {stream, e};
        }

        persist operator<<(std::ostream& stream, const effect_set& e) {
            return {stream, e};
        }

        std::ostream& persist::ostream() {
            return *stream_;
        }

        const std::ostream& persist::ostream() const {
            return *stream_;
        }

        persist::operator std::ostream&() {
            return *stream_;
        }

        persist::operator const std::ostream&() const {
            return *stream_;
        }

        void persist::delete_early() {
            if(location_in_stack_ != empty_persist_location) {
                detail::delete_persist(stream_, location_in_stack_);
            }
            location_in_stack_ = empty_persist_location;
        }


        persist::~persist() {
            delete_early();
        }

        effect_string::string_and_effects& effect_string::back_() {
            if(!strings_.size()) {
                strings_.push_back({});
            }
            return strings_.back();
        }

        const effect_string::string_and_effects& effect_string::back_() const {
            return strings_.back();
        }

        effect_string& effect_string::operator<<(const effect_string& arg) {
            for(auto& string : arg.strings_) {
                if(back_().active_effects == string.active_effects) {
                    back_().string += string.string;
                }
                else {
                    strings_.push_back(std::move(string));
                }
            }

            return *this;
        }

        effect_string& effect_string::operator<<(effect_string&& arg) {

            for(auto& string : arg.strings_) {
                if(back_().active_effects == string.active_effects) {
                    back_().string += std::move(string.string);
                }
                else {
                    strings_.push_back(std::move(string));
                }
            }

            return *this;
        }

        effect_string& effect_string::operator+=(const effect_string& arg) {
            return (*this << arg);
        }

        effect_string& effect_string::operator+=(effect_string&& arg) {
            return (*this << std::move(arg));
        }

        std::string effect_string::unsafe_string(const std::ostream& stream) const {
            std::string ret;
            for(const auto& string : strings_) {
                ret += string.string;

                for(unsigned i = 0; i < string.active_effects.size(); ++i) {
                    if(string.active_effects[i]) {
                        ret += detail::get_top_code(&stream, static_cast<effect_type>(i));
                    }
                }
            }

            return ret;
        }

        persist&& operator<<(persist& p, const effect_string& es) {
            return p << es.unsafe_string(*p.stream_);
        }

        persist operator<<(std::ostream& os, const effect_string& es) {
            persist ret(os);

            ret << es;

            return ret;
        }

        namespace detail {
            #ifdef IRO_UNIX
                bool stdout_isatty() {
                    return isatty(STDOUT_FILENO);
                }
                bool stderr_isatty() {
                    return isatty(STDERR_FILENO);
                }
            #elif defined(IRO_WINDOWS)
                bool stdout_isatty() {
                    return _isatty(_fileno(stdout));
                }
                bool stderr_isatty() {
                    return _isatty(_fileno(stderr));
                }
            #endif

            bool isatty(const std::ostream& os) {
                if(&os == &std::cout) {
                    return stdout_isatty();
                }
                else if(&os == &std::cerr) {
                    return stderr_isatty();
                }
                else {
                    return false;
                }
            }

            effect create(const char* code, effect_type type) noexcept {
                return {code, type};
            }

            struct effect_type_to_stream_hash_t {
                std::size_t operator()(const std::ostream* os) const {
                    if(((os == &std::cout) || (os == &std::cerr)) && stdout_isatty() && stderr_isatty()) {
                        return std::hash<const std::ostream*>()(&std::cout);
                    }
                    else {
                        return std::hash<const std::ostream*>()(os);
                    }
                }
            };

            struct effect_type_to_stream_equals_t {
                std::size_t operator()(const std::ostream* lhs, const std::ostream* rhs) const {
                    if(((lhs == &std::cout) || (lhs == &std::cerr)) && ((rhs == &std::cout) || (rhs == &std::cerr)) && stdout_isatty() && stderr_isatty()) {
                        return true;
                    }
                    else {
                        return (lhs == rhs);
                    }
                }
            };

            struct effect_entry_t {
                std::array<const char*, number_of_effect_types> type_to_code;
                bool is_empty = false; // I'm not a huge fan of there being two distinct empty states, but I can't think of another way to implement the behavior I want
                bool is_destructed = false;

                static effect_entry_t create_empty() {
                    return {filled_array<const char*>(nullptr), true};
                }
            };

            // TODO: write a struct that contains the stack and an array of uints that point to the top non-empty location for each effect type.
            //  That way you don't have to iterate down from the top of the stack every time you want to find the top non-empty location.
            //  Simple space-time tradeoff

            struct stack_and_top_nonempty_location {
                std::vector<effect_entry_t> stack;
                std::array<unsigned, number_of_effect_types> top_nonempty_location;
            };

            static std::unordered_map<const std::ostream*,
                                      std::vector<effect_entry_t>,
                                      effect_type_to_stream_hash_t, effect_type_to_stream_equals_t> stream_to_stack_;

            static std::array<const char*, number_of_effect_types> effect_type_to_default_code_ = {"\x1b[39m",
                                                                                                   "\x1b[49m",
                                                                                                   "\x1b[22m",
                                                                                                   "\x1b[24m",
                                                                                                   "\x1b[25m"};

            static std::ostream* streams_[] = {&std::cout, &std::cerr};

            // returns location in map
            unsigned push_persist(std::ostream* stream, effect_type type, const char* code) {
                auto ret = push_empty_persist(stream);
                set_top(stream, type, code);

                return ret;
            }

            unsigned push_empty_persist(std::ostream* stream) {
                if(!stream_to_stack_.count(stream)) { // I'm using c++14, so .contains() isn't available :(
                    stream_to_stack_[stream].push_back({effect_type_to_default_code_});
                }
                auto& stack = stream_to_stack_.at(stream);
                unsigned ret = stack.size();
                stack.push_back(effect_entry_t::create_empty());

                return ret;
            }

            unsigned push_persist(std::ostream* stream, const effect_set& effects) {
                auto ret = push_empty_persist(stream);
                for(unsigned i = 0; i < number_of_effect_types; ++i) {
                    if(effects.type_to_code_[i]) {
                        set_top(stream, static_cast<effect_type>(i), effects.type_to_code_[i]);
                    }
                }

                return ret;
            }

            unsigned copy_persist(std::ostream* stream, unsigned index_in_stack) {
                auto& stack = stream_to_stack_.at(stream);
                return push_persist(stream, {stack[index_in_stack].type_to_code}); // push calls set, which takes care of setting is_empty, so we don't have to copy it from the old persist's entry manually
            }

            void delete_persist(std::ostream* stream, unsigned index_in_stack) {
                auto& stack = stream_to_stack_.at(stream);

                stack[index_in_stack].is_destructed = true;

                if(index_in_stack == stack.size()-1) {
                    unsigned i = stack.size();

                    while((i > 0) && (stack[i-1].is_destructed)) { // pop all already-destructed persists
                        stack.pop_back();                          // note that we can't pop empty (default constructed) persists, because they're still valid and could be assigned to at any time

                        --i;
                    }
                }

                for(unsigned effect_type_index = 0; effect_type_index < number_of_effect_types; ++effect_type_index) {
                    auto top_code = get_top_code(stream, static_cast<effect_type>(effect_type_index));
                    *stream << top_code;
                    if(stdout_isatty() && stderr_isatty()) { // TODO: put this in a function to get rid of code duplication
                        for(unsigned i = 0; i < 2; ++i) {
                            if(stream == streams_[i]) {
                                *streams_[!i] << top_code;
                                break;
                            }               // ^ !i turns 0 into 1 and 1 into 0,
                                            // so this basically says "if the stream is cout, also print this effect to cerr,
                                            // and if the stream is cerr, also print this effect to cout
                        }
                    }
                }
            }

            void set(std::ostream* stream, unsigned index, effect_type type, const char* code) {
                auto& stack = stream_to_stack_.at(stream);
                stack[index].type_to_code[type] = code;
                stack[index].is_empty = false;

                bool is_top_non_empty = true;
                for(unsigned i = index+1; i < stack.size(); ++i) {
                    is_top_non_empty &= !(stack[i].type_to_code[type]);
                }
                if(is_top_non_empty) {
                    *stream << code;

                    if(stdout_isatty() && stderr_isatty()) {
                        for(unsigned i = 0; i < 2; ++i) {
                            if(stream == streams_[i]) {
                                *streams_[!i] << code;
                                break;
                            }          // ^ !i turns 0 into 1 and 1 into 0,
                                       // so this basically says "if the stream is cout, also print this effect to cerr,
                                       // and if the stream is cerr, also print this effect to cout
                        }
                    }
                }
            }

            void set(std::ostream* stream, unsigned index, const effect_set& effects) {
                for(unsigned i = 0; i < effects.type_to_code_.size(); ++i) {
                    if(effects.type_to_code_[i]) { // TODO: In order to have this work with persist assignment operators, you'll have to change this so that when the code is empty,
                                                   // it cleans up the old persists state. This will involve walking down the stack and finding the last non-empty code for the given effect type
                        set(stream, index, static_cast<effect_type>(i), effects.type_to_code_[i]);
                    }
                }
            }

            bool persist_has_effect_of_type(std::ostream* stream, unsigned index, effect_type type) {
                return stream_to_stack_.at(stream)[index].type_to_code[type];
            }

            void set_top(std::ostream* stream, effect_type type, const char* code) {
                set(stream, stream_to_stack_.at(stream).size()-1, type, code);
            }

            // TODO: maybe change this function so that it just takes a stream and returns a single string with all effect codes
            const char* get_top_code(const std::ostream* stream, effect_type type) {
                if(!stream_to_stack_.count(stream)) { // I'm using c++14, so .contains() isn't available :(
                    stream_to_stack_[stream].push_back({effect_type_to_default_code_});
                }
                auto& stack = stream_to_stack_.at(stream);

                for(unsigned i = stack.size(); i > 0; --i) {
                    if(stack[i-1].type_to_code[type]) { // find first code that isn't null
                        return stack[i-1].type_to_code[type];
                    }
                }

                assert(false);
            }

            void reapply_top(std::ostream* stream, effect_type type) {
                *stream << get_top_code(stream, type);
            }
        }
    #endif
}

//end of "iro.h" include


namespace cppli {
    namespace detail {
        void print_throw_or_do_nothing(minor_error_type error_type,
                                       const std::string& print_if_error_or_mesasge,
                                       const std::string& print_only_if_message) {
            if(get_minor_error_behavior(error_type) == THROW) {
                throw cli_error(print_if_error_or_mesasge, error_type);
            }
            else if(get_minor_error_behavior(error_type) == MESSAGE) {
                std::cerr << iro::bright_yellow << iro::effect_string(iro::bold|iro::underlined, "Warning:") << ' ' << print_if_error_or_mesasge << print_only_if_message;
            }
        }


        std::unordered_map<minor_error_type, error_behavior>& minor_error_code_to_behavior_() {
            static std::unordered_map<minor_error_type, error_behavior> minor_error_code_to_behavior;

            if(!minor_error_code_to_behavior.size()) {
                for(unsigned i = 0; i < NUMBER_OF_MINOR_ERROR_TYPES; ++i) {
                    minor_error_code_to_behavior.emplace(static_cast<minor_error_type>(i), MESSAGE);
                }
            }

            return minor_error_code_to_behavior;
        }
    }


    void           set_minor_error_behavior(minor_error_type type, error_behavior behavior) {
        detail::minor_error_code_to_behavior_().at(type) = behavior;
    }

    error_behavior get_minor_error_behavior(minor_error_type type) {
        return detail::minor_error_code_to_behavior_().at(type);
    }
}
//end of "configuration.cpp" include

//included from file "../src/arg_parsing.cpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <format>


//included from file "../include/cppli/detail\arg_parsing.h"


namespace cppli::detail {
    struct parse_ret_t {
        std::vector<command_t> subcommands;
        bool printed_help = false;
        std::optional<unsigned> help_command_index;
    };

    parse_ret_t parse(const std::vector<std::string>& arg_vec);

    std::vector<std::string> argv_to_arg_vec(int argc, const char* const* argv);
    parse_ret_t parse(int argc, const char* const* const argv);
}
//end of "arg_parsing.h" include

namespace cppli::detail {
    bool contains_letters(const std::string& str) {
        bool contains = false;
        for(char c : str) {
            contains |= isletter(c);
        }

        return contains;
    }

    std::string say_something_if_empty(const std::string& arg) {
        if(!arg.size()) {
            return "(empty string)";
        }
        else {
            return '\"' + arg + '\"';
        }
    }
    
    bool all_lowercase_numeral_or_hyphen_and_first_char_letter(const std::string& str) {
        bool all_valid = true;

        for(char c : str) {
            all_valid &= (islowercase(c) ||
                          is_numeral(c)  ||
                         (c == '-'));
        }

        return all_valid && str.size() && islowercase(str[0]);
    }

        // TODO: break each major if in the loop into a function, and put each function in its own file
    parse_ret_t parse(const std::vector<std::string>& arg_vec) {



        /*if(program_name() != argv[0]) { // TODO: switch to enable/disable this warning. I'm commenting it out for now because it's annoying
            std::cerr << "The name used to invoke the program (\"" << argv[0] << "\") was not the same as the program's documented name (\"" << program_name() << "\")\n";
        }*/

        // skip the program name
        command_name_t subcommand_name = {"MAIN"};

        //command_inputs_t args;
        std::vector<command_t> commands{{subcommand_name}};

        bool disambiguate_next_arg = false;
        bool disambiguate_until_subcommand = false;
        bool disambiguate_all = false;

        bool invalid_input_to_namespace = false;
                                                    // possibly the longest variable name I've ever written
        std::unordered_map<unsigned, std::string> optional_argument_option_with_no_value_provided_arg_index_to_option_string; // indices in argv of optional argument optionals that weren't provided an argument
                                                                                                                              // used to make better error messages
        std::string first_command_name = arg_vec[0];

        bool in_namespace = is_namespace({"MAIN"});

        std::optional<unsigned> help_command_index;

        std::string command_or_subcommand = "main command";

        std::string current_subcommand_name_string = to_string(subcommand_name);

        parse_ret_t ret;

        for(unsigned arg_i = 1; arg_i < arg_vec.size(); ++arg_i) {
            std::string arg_string = arg_vec[arg_i];

            if(is_valid_subcommand(subcommand_name, arg_string) && !(disambiguate_next_arg || disambiguate_all)) {
                if(arg_string == "help") {
                    ret.help_command_index = commands.size();
                }
                in_namespace = is_namespace(subcommand_name);

                command_or_subcommand = "subcommand";
                current_subcommand_name_string = to_string(subcommand_name);

                disambiguate_until_subcommand = false;

                //commands.back().inputs = std::move(args);
                //args = {};

                commands.push_back({subcommand_name});
            }
            else {
                command_inputs_t& args = commands.back().inputs;
                /*if(in_namespace) { // I forgot why this doesn't work
                    std::cerr << '\"' << current_subcommand_name_string << "\" is a namespace, so the only inputs it can accept are --help, -h, or help. The given input \"" << arg_string << "\" will therefore be ignored\n";
                    continue;
                }*/
                if((arg_string.substr(0,2) == "--") && !(disambiguate_next_arg || disambiguate_until_subcommand || disambiguate_all)) { // long flag (these are ez)
                                                    // we need this check so that we can handle the case where "--" is the thing we're trying to disambiguate. Ex: "program -- --"
                    if(arg_string[2] == '1') { // arg_string == "--1"
                        disambiguate_next_arg = true;
                    }
                    else if(arg_string.size() == 2) { // arg_string == "--"
                        disambiguate_until_subcommand = true;
                    }
                    else if(arg_string[2] == '-') { // arg_string == "---"
                        disambiguate_all = true;
                    }
                    else {
                        std::string::size_type equals_pos;
                        if((equals_pos = arg_string.find('=')) != std::string::npos) { // TODO: make sure the -2 doesn't cause issues
                            std::string option_name  = arg_string.substr(2, equals_pos-2),
                                        option_value = arg_string.substr(equals_pos+1, arg_string.size()-(equals_pos+1));

                            if(subcommand_takes_option(subcommand_name, option_name)) {
                                error_if_flag_or_option_already_included(commands.back(), option_name);
                                args.options_to_values.emplace(option_name, option_value);
                            }
                            else if(subcommand_takes_flag(subcommand_name, option_name)) {
                                std::stringstream ss;
                                ss << "For " << command_or_subcommand << " \"" << current_subcommand_name_string << "\", \"" << option_name << "\" is a flag, not an option, and therefore can't be assigned a value (like it was in \"" << arg_string << "\"). ";

                                print_throw_or_do_nothing(FLAG_GIVEN_AN_ARGUMENT, ss.str(), "The value will be ignored and the flag will be set to true\n\n");

                                args.flags.emplace(option_name);
                            }
                            else {
                                if(option_name == "help") {
                                    get_print_documentation_string_callback()(subcommand_name, default_top_level_help_verbosity, default_subcommand_help_verbosity, default_help_recursion_level, default_hide_help_status);
                                    return {{}, true};
                                }
                                else if(in_namespace) {
                                    std::stringstream ss;
                                    ss << '\"' << current_subcommand_name_string << "\" is a namespace, so it cannot accept the input \"" << arg_string << "\" (the only inputs it can accept are --help, -h, and help)";

                                    print_throw_or_do_nothing(ARGUMENT_GIVEN_TO_NAMESPACE, ss.str(), "The given input be ignored\n\n");
                                    invalid_input_to_namespace = true;
                                    continue;
                                }
                                else {
                                    std::stringstream ss;
                                    ss << "Flag/option \"" << option_name << "\" (from \"" << arg_string << "\") was not recognized by " << command_or_subcommand << " \"" << current_subcommand_name_string << "\"\n";

                                    print_throw_or_do_nothing(UNRECOGNIZED_FLAG, ss.str(), "It will be ignored\n\n");
                                }
                            }
                        }
                        else {
                            std::string option_or_flag_name = arg_string.substr(2, arg_string.size()-1);

                            {
                                bool all_letter_or_hyphen = true;
                                for(char c : option_or_flag_name) {
                                    all_letter_or_hyphen &= (isletter(c) || (c == '-'));
                                }

                                if(!all_letter_or_hyphen) {
                                    // TODO: rename INVALID_FLAG to be more general (because technically we don't konw if it's a flag or an option)
                                    print_throw_or_do_nothing(INVALID_FLAG, std::format("Flag or option \"{}\" contains characters other than letters and hyphens, and therefore cannot form a valid flag/option", option_or_flag_name), ". It will be ignored\n\n");
                                    continue;
                                }
                            }

                            //error_if_flag_or_option_already_included(commands.back(), option_or_flag_name);

                            if(subcommand_takes_option(subcommand_name, option_or_flag_name)) {
                                error_if_flag_or_option_already_included(commands.back(), option_or_flag_name);

                                if(subcommand_option_argument_is_optional(subcommand_name, option_or_flag_name)) {
                                    args.options_to_values.emplace(option_or_flag_name, std::nullopt);
                                    optional_argument_option_with_no_value_provided_arg_index_to_option_string.emplace(arg_i, arg_string);
                                }
                                else {
                                    if(arg_i+1 < arg_vec.size()) {
                                        args.options_to_values.emplace(option_or_flag_name, arg_vec[arg_i+1]);
                                        ++arg_i; // we just ate the next arg, so don't process it again
                                    }
                                    else {
                                        std::stringstream ss;
                                        ss << "Argument \"" << arg_string
                                           << "\" given to " << command_or_subcommand << ' ' << current_subcommand_name_string
                                           << " referred to an option with a required argument, but no argument followed\n";

                                        throw cli_error(ss.str(), OPTION_REQUIRED_ARGUMENT_NOT_PROVIDED);
                                    }
                                }
                            }
                            else if(subcommand_takes_flag(subcommand_name, option_or_flag_name)) {
                                error_if_flag_or_option_already_included(commands.back(), option_or_flag_name);
                                args.flags.emplace(option_or_flag_name);
                            }
                            else {
                                if(option_or_flag_name == "help") {
                                    get_print_documentation_string_callback()(subcommand_name, default_top_level_help_verbosity, default_subcommand_help_verbosity, default_help_recursion_level, default_hide_help_status);
                                    return {{}, true};
                                }
                                else if(in_namespace) {
                                    std::cerr << iro::bright_yellow << iro::effect_string(iro::bold|iro::underlined, "Warning:") <<  " \"" << current_subcommand_name_string << "\" is a namespace, so the only inputs it can accept are --help, -h, or help. The given input \"" << arg_string << "\" will therefore be ignored\n";
                                    invalid_input_to_namespace = true;
                                    continue;
                                }
                                else {
                                    print_throw_or_do_nothing(INVALID_FLAG, std::format(R"({} "{}" does not accept a flag or option "{}")", command_or_subcommand, current_subcommand_name_string, arg_string), " It will be ignored\n\n");
                                }
                            }
                        }
                    }
                }                                  // so that string like "-" and " - " can be used as positionals without issue
                else if((arg_string[0] == '-') && (arg_string.size() > 1) && isletter(arg_string[1]) && !(disambiguate_next_arg || disambiguate_until_subcommand || disambiguate_all)) { // short flag(s) and/or option (these are not so ez)
                    bool invalid_character_in_flag_group = false;            // don't consider this arg a flag group if it starts with an invalid char (not a letter)
                    unsigned invalid_character_index;                        // allows the user to enter negative numbers as positional arguments with a positional separator
                    std::stringstream invalid_character_in_flag_group_message;

                    for(unsigned char_i = 1; char_i < arg_string.size(); ++char_i) {
                        std::string char_string = arg_string.substr(char_i,1);

                        if(subcommand_takes_option(subcommand_name, char_string)) { // there is an argument
                            error_if_short_flag_or_option_already_included(commands.back(), char_string);
                            if(char_i < arg_string.size()-1) { // no equals sign, so everything after this character is the argument    // TODO: maybe check to see if the string contains any flags with required args, and then if not we can employ this logic
                                args.options_to_values.emplace(char_string, arg_string.substr(char_i+1+(arg_string[char_i+1] == '='), arg_string.size()));
                                break;                                                                 // ^ discard a leading '='!!
                            }
                            else if(char_i == arg_string.size()-1) { // no room for argument
                                if(subcommand_option_argument_is_optional(subcommand_name, char_string)) { // if this option's argument is optional, assume that no argument is provided, and that the next arg is unrelated
                                    args.options_to_values.emplace(char_string, std::nullopt);
                                    optional_argument_option_with_no_value_provided_arg_index_to_option_string.emplace(arg_i, "-" + char_string);
                                }
                                else {
                                    if(arg_i+1 < arg_vec.size()) {
                                        args.options_to_values.emplace(char_string, arg_vec[arg_i+1]);
                                        ++arg_i; // we just ate the next arg, so don't process it again
                                    }
                                    else {
                                        std::stringstream ss;
                                        ss << "The last character '" << arg_string[char_i]
                                           << "' in flag/option group \"" << arg_string << "\" "
                                              "given to " << command_or_subcommand << ' ' << current_subcommand_name_string
                                           << " referred to an option with a required argument, but no argument followed\n";

                                        throw cli_error(ss.str(), OPTION_REQUIRED_ARGUMENT_NOT_PROVIDED);
                                    }
                                }
                            }
                            else {
                                assert(false); // this should never happen (would mean char_i is out of range of arg_string)
                            }
                        }
                        else if(subcommand_takes_flag(subcommand_name, char_string)) {
                            error_if_short_flag_or_option_already_included(commands.back(), char_string);
                            args.flags.emplace(char_string);
                        }
                        else if(char_string == "h") {
                            get_print_documentation_string_callback()(subcommand_name, default_top_level_help_verbosity, default_subcommand_help_verbosity, default_help_recursion_level, default_hide_help_status);
                            return {{}, true};
                        }
                        else if(in_namespace) {
                            std::cerr << '\"' << current_subcommand_name_string << "\" is a namespace, so the only inputs it can accept are --help, -h, or help. The given input \"" << arg_string << "\" will therefore be ignored\n";
                            invalid_input_to_namespace = true;
                            continue;
                        }
                        else {
                            if(!isletter(char_string[0])) {
                                if(!invalid_character_in_flag_group) {
                                    invalid_character_index = char_i; // because we only want to set this the first time we find an invalid character
                                }
                                invalid_character_in_flag_group = true;
                                invalid_character_in_flag_group_message << "Character '" << char_string << "' in flag/option group \"" << arg_string << "\" "
                                   << "given to " << command_or_subcommand << " \"" << current_subcommand_name_string << "\" "
                                   << "is not a letter and therefore cannot form a valid short name for a flag or option\n";

                                if(arg_string[invalid_character_index] == '=') {
                                    break; // so that we don't continue to look for flags (valid or invalid) in the "argument"
                                }
                            }
                            else {
                                std::stringstream ss;
                                ss << "Character '" << char_string << "' in flag/option group \"" << arg_string << "\" "
                                      "was not a recognized flag or option for " << command_or_subcommand << " \"" << current_subcommand_name_string << "\".";

                                print_throw_or_do_nothing(UNRECOGNIZED_FLAG, ss.str(), " It will be ignored\n\n");
                            }
                        }
                    }

                    if(invalid_character_in_flag_group) {
                        if((invalid_character_index > 1) && subcommand_takes_flag(subcommand_name, {arg_string[invalid_character_index-1]})) { // 1 instead of 0 because of the leading '-'
                            if(arg_string[invalid_character_index] == '=') {
                                std::stringstream ss;
                                ss << "Character '" << arg_string[invalid_character_index-1] << "' in flag/option group \"" << arg_string << "\" is a flag, and therefore can't accept an argument.\n";

                                print_throw_or_do_nothing(FLAG_GIVEN_AN_ARGUMENT, ss.str(),
                                                          "The argument " + say_something_if_empty(arg_string.substr(invalid_character_index+1, arg_string.size())) +
                                                          " will be ignored and the flag '" + arg_string[invalid_character_index-1] + "' will be set to true\n");
                                continue;
                            }
                            else {
                                invalid_character_in_flag_group_message << "It's possible that the substring \"" << arg_string.substr(invalid_character_index, arg_string.size())
                                                                        << "\" was meant to be an argument for '" << arg_string[invalid_character_index-1] << "', but '"
                                                                        << arg_string[invalid_character_index-1] << "' is flag and therefore can't have an argument.\n";
                            }
                        }
                        print_throw_or_do_nothing(INVALID_FLAG, invalid_character_in_flag_group_message.str(), "Invalid flag(s) will be ignored\n\n");
                    }

                }
                else { // positional arg
                    if(in_namespace) {
                        std::cerr << '\"' << current_subcommand_name_string << "\" is a namespace, so the only inputs it can accept are --help, -h, or help. The given input \"" << arg_string << "\" will therefore be ignored\n";
                        invalid_input_to_namespace = true;
                        continue;
                    }
                    else {
                        disambiguate_next_arg = false;
                        args.positional_args.emplace_back(arg_string);
                    }

                    const auto& docs = subcommand_name_to_docs()[commands.back().name];
                    const auto& command = commands.back();

                    unsigned expected_positionals_count = docs.positionals.size(),
                             actual_positionals_count   = args.positional_args.size();

                    if(!docs.variadic && (actual_positionals_count > expected_positionals_count)) {

                        std::stringstream ss;
                        ss << "Unexpected positional argument \"" << arg_string
                           << "\" given to " << (command.name == command_name_t{"MAIN"} ? "main command" : "subcommand") << ' ' << to_string(command.name) << '\n';/* <<
                           "\" (expected " << expected_positionals_count << ", got " << actual_positionals_count << ").\n";*/

                        if(all_lowercase_numeral_or_hyphen_and_first_char_letter(arg_string)) {
                            ss << "It's also possible that \"" << arg_string
                               << "\" was supposed to be a subcommand, but was not recognized. "
                                  "You can run\n" << to_string(command.name, " ")
                               << " help\nto view the available subcommands for this command\n";
                        }

                        minor_error_type e;
                        if(optional_argument_option_with_no_value_provided_arg_index_to_option_string.contains(arg_i-1)) {
                            const std::string& erroneous_option_string = optional_argument_option_with_no_value_provided_arg_index_to_option_string.at(arg_i-1);
                            ss << "\nAnother possibility is that \"" << arg_string << "\" was supposed to be an argument for the previous commandline argument "
                               << '\"' << optional_argument_option_with_no_value_provided_arg_index_to_option_string.at(arg_i - 1) << '\"'
                               << ", which is an option that accepts an optional argument.\n"
                                  "However, an optional argument option can't be given an argument using the space separated syntax.\n"
                                  "The argument must use the '=' syntax (" << erroneous_option_string << '=' << arg_string << ')';

                            if(arg_vec[arg_i-1].substr(0,2) != "--") {
                                ss << " or, for (potentially grouped) short options, the connected syntax ("
                                   << erroneous_option_string << arg_string << ")";
                            }

                            ss << '\n';

                            e = EXCESS_POSITIONAL_WITH_SUSPICIOUS_OPTION;
                        }
                        else {
                            e = EXCESS_POSITIONAL;
                        }


                        print_throw_or_do_nothing(e, ss.str(), "This argument will be ignored\n\n");
                    }
                }
            }
        }

        if(is_namespace(commands.back().name)) {
            /*if(invalid_input_to_namespace) {
                std::cout << "Here is its help page:\n";
            }
            else {*/
            if(!invalid_input_to_namespace) { // TODO: make it configurable whether invalid input to namespace just gives a warning, or causes the program to stop and print help
                std::cout << '\"' << to_string(commands.back().name)
                          << "\" is a namespace, so using it without further subcommands doesn't do anything. Here is its help page: \n";
                //}
                get_print_documentation_string_callback()(commands.back().name, default_top_level_help_verbosity, default_subcommand_help_verbosity,
                                                                 default_help_recursion_level, default_hide_help_status);
                return {{}, true};
            }
        }

        ret.subcommands = std::move(commands);
        return ret;
    }

    std::vector<std::string> argv_to_arg_vec(int argc, const char* const* argv) {
        if(argc == 0) {
            std::cerr << iro::bright_red << iro::effect_string(iro::bold|iro::underlined, "Error:") << " argc == 0. This is very terrible and unrecoverable\n";
            std::exit(-1);
        }
        if(!argv[0]) {
            std::cerr << iro::bright_red << iro::effect_string(iro::bold|iro::underlined, "Error:") << " argv[0] was null. This is very terrible and unrecoverable\n";
            std::exit(-1);
        }

        std::vector<std::string> arg_vec(argc);
        for(unsigned i = 0; i < argc; ++i) {
            arg_vec[i] = argv[i];
        }
        return arg_vec;
    }

    parse_ret_t parse(int argc, const char* const* argv) {
        return parse(argv_to_arg_vec(argc, argv));
    }
}
//end of "arg_parsing.cpp" include

//included from file "../src/documentation.cpp"

#include <format>

#define ORI_IMPL

//included from file "../lib/ori\ori.h"

#include <iostream>
#include <sstream>
#include <cassert>
#include <unordered_map>

#if defined(__unix__) || defined(__unix) || defined(__APPLE__) || defined(__MACH__)
    #define ORI_UNIX
    #include <unistd.h>
    #include <sys/ioctl.h>
#elif defined(_WIN32) || defined(__WIN32__)
    #define ORI_WINDOWS
    #include <windows.h>
    #include <io.h>
#else
    #error "Platform not supported. Sorry!\n";
#endif

namespace ori {
    namespace detail {
        #ifdef ORI_IMPL
            #ifdef ORI_UNIX
                int get_file_descriptor_(const std::ostream& stream) {
                    int fd;
                    if(&stream == &std::cout) {
                        if(isatty(fileno(stdout))) {
                            fd = fileno(stdout);
                        }
                        else {
                            throw std::runtime_error("get_term_width error: output stream did not refer to a tty");
                        }
                    }
                    else if(&stream == &std::cerr) {
                        if(isatty(fileno(stderr))) {
                            fd = fileno(stderr);
                        }
                        else {
                            throw std::runtime_error("get_term_width error: output stream did not refer to a tty");
                        }
                    }
                    else {
                        throw std::runtime_error("get_term_width error: output stream was not cout or cerr");
                    }

                    return fd;
                }

                unsigned get_term_width_(const std::ostream& stream) {
                    struct winsize w;
                    try {
                        assert(ioctl(get_file_descriptor_(stream), TIOCGWINSZ, &w) >= 0);
                    }
                    catch(std::runtime_error&) {
                        return -1; // the stream wasn't cout, cerr, or a tty. This means we can't do any fancy wrapping, so just return -1 (uint_max), effectively disabling wrapping
                    }
                    //return 205;

                    return w.ws_col;
                }
            #endif

            #ifdef ORI_WINDOWS
                HANDLE get_file_handle_(const std::ostream& stream) {
                    DWORD handle;
                    if(&stream == &std::cout) {
                        if(_isatty(fileno(stdout))) {
                            handle = STD_OUTPUT_HANDLE;
                        }
                        else {
                            throw std::runtime_error("get_term_width error: output stream did not refer to a tty");
                        }
                    }
                    else if(&stream == &std::cerr) {
                        if(_isatty(fileno(stderr))) {
                            handle = STD_ERROR_HANDLE;
                        }
                        else {
                            throw std::runtime_error("get_term_width error: output stream did not refer to a tty");
                        }
                    }
                    else {
                        throw std::runtime_error("get_term_width error: output stream was not cout or cerr");
                    }

                    return GetStdHandle(handle);
                }

                unsigned get_term_width_(const std::ostream& stream) {
                    CONSOLE_SCREEN_BUFFER_INFO csbi; // implementation comes from https://stackoverflow.com/a/12642749

                    try {
                        assert(GetConsoleScreenBufferInfo(get_file_handle_(stream), &csbi));
                    }
                    catch(std::runtime_error&) {
                        return -1; // the stream wasn't cout, cerr, or a tty. This means we can't do any fancy wrapping, so just return -1 (uint_max), effectively disabling wrapping
                    }


                    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
                }
            #endif

            static unsigned current_indent_ = 0;


            static std::unordered_map<const std::ostream*, unsigned> stream_index_in_current_line_; // TODO: instead of a bool, have an unsigned that keeps track of the current line
                                                                                                // length, so that subsequent print statements don't wrap too late
            unsigned last_char_index_in_line_(const std::ostream& stream) {

                if(!stream_index_in_current_line_.count(&stream)) {
                    stream_index_in_current_line_.emplace(&stream, 0);
                }

                return stream_index_in_current_line_.at(&stream);
            }

            void set_last_char_index_in_line_(const std::ostream& stream, unsigned value) {
                stream_index_in_current_line_[&stream] = value;
            }


            void print_impl_(std::ostream& stream, const std::string& in_str, unsigned indent, unsigned right_padding, bool hyphenate_cutoffs) {
                if(indent == -1) {
                    indent = current_indent_;
                }
                // TODO: should probably trim whitespace from the end of in_str

                std::string out_str(!bool(last_char_index_in_line_(stream)) * indent, ' ');

                unsigned term_width = detail::get_term_width_(stream);
                unsigned max_line_width = term_width - indent - right_padding;

                unsigned index_in_current_line = last_char_index_in_line_(stream);
                unsigned current_line_start_i = 0;
                bool is_first_line = true;
                for(unsigned source_i = 0; source_i < in_str.size(); ++source_i) {
                    if(in_str[source_i] == '\n') {
                        out_str += in_str.substr(current_line_start_i, source_i - current_line_start_i /*- is_first_line*last_char_index_in_line_(stream)*/);

                        if((source_i != in_str.size()-1) && (in_str[source_i+1] != '\n')) {
                            out_str += '\n';
                        }

                        for(unsigned space_i = 0; space_i < indent; ++space_i) {
                            out_str += ' ';
                        }

                        if((source_i + 1 < in_str.size()) && (in_str[source_i + 1] != '\n')) {
                            ++source_i;
                        }

                        index_in_current_line = 0;
                        current_line_start_i = source_i;

                        is_first_line = false;
                    }
                    else if(index_in_current_line == max_line_width) {
                        for(unsigned j = source_i; j > current_line_start_i; --j) {

                            if(std::isspace(in_str[j])) {
                                out_str += in_str.substr(current_line_start_i, j - current_line_start_i - is_first_line*last_char_index_in_line_(stream));
                                out_str += '\n';
                                for(unsigned space_i = 0; space_i < indent; ++space_i) {
                                    out_str += ' ';
                                }

                                for(unsigned k = j; k < in_str.size(); ++k) {
                                    if(!std::isspace(in_str[k])) {
                                        source_i = k;

                                        index_in_current_line = 0;
                                        current_line_start_i = source_i;
                                        is_first_line = false;

                                        goto end_of_first_loop;
                                    }
                                }
                            }
                        }
                        // if we reach this code, that means that there were no spaces in the line, so we have no choice but to cut it off
                        out_str += in_str.substr(current_line_start_i, max_line_width - 1 - is_first_line*detail::last_char_index_in_line_(stream));
                        if(hyphenate_cutoffs) {
                            out_str += '-';

                        }
                        if(source_i) {
                            --source_i;
                        }

                        if(source_i < in_str.size() - 1) {
                            out_str += '\n';
                            for(unsigned space_i = 0; space_i < indent; ++space_i) {
                                out_str += ' ';
                            }
                        }

                        index_in_current_line = 0;
                        current_line_start_i = source_i;

                        is_first_line = false;
                        goto end_of_first_loop;
                    }

                    end_of_first_loop:;
                    ++index_in_current_line;
                }
                if(in_str.size() - current_line_start_i) {
                    out_str += in_str.substr(current_line_start_i, std::string::npos);
                    set_last_char_index_in_line_(stream, (out_str.back() != '\n')*(in_str.size() - current_line_start_i));
                }
                else {
                    set_last_char_index_in_line_(stream, 0);
                }

                stream << out_str;
            }

            const std::string& to_string_(const std::string& str) {
                return str;
            }

            std::string to_string_(const char* cstr) {
                return {cstr};
            }

            std::string to_string_(char c) {
                   return {1, c};
            }

        #else // end of #ifdef ORI_IMPL block
            void print_impl_(std::ostream& stream, const std::string& in_str, unsigned indent, unsigned right_padding, bool hyphenate_cutoffs);
            const std::string& to_string_(const std::string& str);
            std::string to_string_(const char* cstr);
            std::string to_string_(char c);
        #endif

        template<typename T>
        std::string to_string_(const T& arg) {
            std::stringstream ss; // uh oh probably very slow
            ss << arg;
            return ss.str();
        }
    }

    #ifdef ORI_IMPL
        void set_indent(unsigned indent) {
            detail::current_indent_ = indent;
        }

        unsigned get_indent() {
            return detail::current_indent_;
        }

        void change_indent(int difference) {
            detail::current_indent_ += difference;
        }


        void println(std::ostream& stream) {
            stream << '\n';
            detail::set_last_char_index_in_line_(stream, 0);
        }

        void println() {
            println(std::cout);
        }
    #endif

    constexpr unsigned use_global_indent = -1; // this is well-defined

    void set_indent(unsigned indent);
    unsigned get_indent();

    /// adds difference to current indent
    void change_indent(int difference);


    template<typename T>
    void print(std::ostream& stream, const T& val, unsigned indent = use_global_indent, unsigned right_padding = 0, bool hyphenate_cutoffs = false) {
        decltype(auto) in_str = detail::to_string_(val);
        detail::print_impl_(stream, in_str, indent, right_padding, hyphenate_cutoffs);
    }

    /// default stream is std::cout
    template<typename T>
    void print(const T& val, unsigned indent = use_global_indent, unsigned right_padding = 0, bool hyphenate_cutoffs = false) {
        print(std::cout, val, indent, right_padding, hyphenate_cutoffs);
    }

    template<typename T>
    void println(std::ostream& stream, const T& val, unsigned indent = use_global_indent, unsigned right_padding = 0, bool hyphenate_cutoffs = false) {
        print(stream, val, indent, right_padding, hyphenate_cutoffs);
        stream << '\n';
        detail::set_last_char_index_in_line_(stream, 0);
    }

    /// default stream is std::cout
    template<typename T>
    void println(const T& val, unsigned indent = use_global_indent, unsigned right_padding = 0, bool hyphenate_cutoffs = false) {
        print(val, indent, right_padding, hyphenate_cutoffs);
        std::cout << '\n';
        detail::set_last_char_index_in_line_(std::cout, 0);
    }

    /// print a newline to the given stream
    void println(std::ostream& stream);

    /// print a newline to std::cout
    void println();
}

#undef ORI_UNIX
#undef ORI_WINDOWS
//end of "ori.h" include

#define IRO_IMPL


// TODO: switch long name short name order, like this [-v/--verbose]
//                                                    [-h/--help]
//                                                    [-f/--force]
namespace cppli {
    constinit documentation_verbosity default_top_level_help_verbosity = NAME_DESCRIPTION_AND_ARGS_WITH_ARG_DESCRIPTIONS;
    constinit documentation_verbosity default_subcommand_help_verbosity = NAME_AND_DESCRIPTION;
    constinit unsigned default_help_recursion_level          = -1;
    constinit bool     default_hide_help_status              = true;


    bool flag_documentation_t::operator<(const flag_documentation_t& rhs) const {
        return (name < rhs.name);
    }

    flag_documentation_t::flag_documentation_t(const std::string& name, const std::string& documentation, char short_name) : name(name),
                                                                                                                             documentation(
                                                                                                                                     documentation),
                                                                                                                             short_name(
                                                                                                                                     short_name) {}

    bool option_documentation_t::operator<(const option_documentation_t& rhs) const {
        return std::tie(is_optional, argument_is_optional, name) <
               std::tie(rhs.is_optional, rhs.argument_is_optional, rhs.name);
    }

    option_documentation_t::option_documentation_t(const std::string& type, const std::string& name, const std::string& argument_text,
                                                   const std::string& documentation, char short_name, bool is_optional,
                                                   bool argument_is_optional) : type(type), name(name), argument_text(argument_text),
                                                                                documentation(documentation), short_name(short_name),
                                                                                is_optional(is_optional),
                                                                                argument_is_optional(argument_is_optional) {}

    positional_documentation_t::positional_documentation_t(const std::string& type, const std::string& name,
                                                           const std::string& documentation, bool optional) : type(type), name(name),
                                                                                                              documentation(documentation),
                                                                                                              optional(optional) {}

    variadic_documentation_t::variadic_documentation_t(const std::string& type, const std::string& name, const std::string& documentation) : type(type),
                                                                                                                                             name(name),
                                                                                                                                             documentation(documentation) {}


    command_documentation_t::command_documentation_t(const std::string& name, const char* description) : name(name), description(description), is_namespace(false) {}

    bool command_documentation_t::operator<(const command_documentation_t& rhs) const {
        return (name.back() < rhs.name.back());
    }

    namespace detail {
        std::unordered_map<command_name_t, command_documentation_t, command_name_hash_t>& subcommand_name_to_docs() {
            static std::unordered_map<command_name_t, command_documentation_t, command_name_hash_t> subcommand_name_to_docs_;

            return subcommand_name_to_docs_;
        }

        std::string add_appropriate_brackets(const std::string& source, bool optional) {
            std::string ret = (optional ? "[" : "<");

            ret += source;

            ret += (optional ? "]" : ">");

            return ret;
        }

        struct name_and_description_t {
            std::string name,
                        description;
        };

        void print_documentation_string_impl(const command_name_t& name,
                                                  const documentation_verbosity& top_level_verbosity, const documentation_verbosity& subcommand_verbosity,
                                                  unsigned max_recursion_level,
                                                  unsigned current_recursion_level,
                                                  bool hide_help,
                                                  const std::optional<name_and_description_t>& main_command_override_name_and_description = std::nullopt) {

            const documentation_verbosity& verbosity = ((current_recursion_level == 0) ? top_level_verbosity : subcommand_verbosity);


            using namespace detail;
            const auto& docs = subcommand_name_to_docs()[name];

            std::string indent = std::string(current_recursion_level*8, ' ');
            //std::string ret = indent;

            ori::change_indent(4);

            if(main_command_override_name_and_description.has_value()) {
                ori::print("Options or arguments surrounded by square brackets are optional, ones surrounded by angular brackets are required.\n"
                           "Arguments of the form [arg:type...] are variadic (as indicated by the \"...\") and can receive any number of arguments (including 0)\n\n");
            }


            struct arg_name_and_docs_t {
                std::string name,
                            docs;

                inline arg_name_and_docs_t(const std::string& name, const std::string& docs) : name(name), docs(docs) {}
            };

            if(docs.is_namespace) {
                std::string name_line;
                if(main_command_override_name_and_description.has_value()) {
                    name_line = (iro::effect_string(iro::bold|iro::underlined, "(Main Namespace)") + ' ').unsafe_string(std::cout);
                }
                else {
                    name_line = (iro::effect_string(iro::bold|iro::underlined, "(Namespace)") + ' ').unsafe_string(std::cout);
                }
                name_line += iro::effect_string(iro::bold|iro::underlined, docs.name).unsafe_string(std::cout);
                ori::print(name_line);
                if(docs.description.size()) {
                    //ori::print('\n');
                    ori::change_indent(4);
                    //ret += indent;
                    ori::print(iro::effect_string(iro::bold, " Description:"));
                    //ret += indent;
                    ori::change_indent(4);
                    //ret += EIGHT_SPACES;
                    ori::print(docs.description); // was  ret == docs.description;
                    ori::change_indent(-8);
                }
                ori::print('\n');
            }
            else {
                std::string name_and_usage_line;
                if(main_command_override_name_and_description.has_value()) {
                    name_and_usage_line = (iro::effect_string(iro::bold|iro::underlined, "(Main Command)") + ' ').unsafe_string(std::cout);
                    name_and_usage_line += ' ';
                }
                else {
                    name_and_usage_line = (iro::effect_string(iro::bold|iro::underlined, "(Subcommand)") + ' ').unsafe_string(std::cout);
                }
                name_and_usage_line += iro::effect_string(iro::bold|iro::underlined, docs.name).unsafe_string(std::cout);
                //ori::print(name_and_usage_line);

                std::vector<arg_name_and_docs_t> positional_doc_strings;
                std::vector<arg_name_and_docs_t> flag_doc_strings;
                std::vector<arg_name_and_docs_t> option_doc_strings;

                std::string variadic_str;
                if(verbosity == NAME_ONLY) {
                    ori::print(name_and_usage_line);
                }
                else if (verbosity >= NAME_AND_ARGS) {
                    name_and_usage_line += ' ';

                    for (const auto& e: docs.positionals) {
                        std::string temp = add_appropriate_brackets(e.name + ":" + e.type, e.optional);
                        name_and_usage_line += temp;
                        name_and_usage_line += ' ';

                        positional_doc_strings.emplace_back(std::move(temp), e.documentation);
                    }

                    if(docs.variadic) {
                        variadic_str = std::format("[{}:{}...]", docs.variadic->name, docs.variadic->type);
                        name_and_usage_line += variadic_str;
                        name_and_usage_line += ' ';
                    }

                    for (const auto& e: docs.flags) {
                        std::string temp = "[--";
                        temp += e.name;
                        if (e.short_name) {
                            temp += "/-";
                            temp += e.short_name;
                        }
                        temp += "]";
                        name_and_usage_line += temp;
                        name_and_usage_line += ' ';

                        flag_doc_strings.emplace_back(std::move(temp), e.documentation);
                    }

                    for (const auto& e: docs.options) {
                        std::string temp = "--";
                        temp += e.name;
                        if (e.short_name) {
                            temp += "/-";
                            temp += e.short_name;
                        }
                        temp += "=";
                        temp += add_appropriate_brackets(e.argument_text + ":" + e.type, e.argument_is_optional);

                        option_doc_strings.emplace_back(add_appropriate_brackets(temp, e.is_optional), e.documentation);

                        name_and_usage_line += option_doc_strings.back().name;
                        name_and_usage_line += ' ';
                    }
                }

                ori::println(name_and_usage_line);

                if ((verbosity == NAME_AND_DESCRIPTION) || (verbosity > NAME_AND_ARGS)) {
                    //ret += indent;
                    ori::change_indent(4);
                    ori::println(iro::effect_string(iro::bold, "Description:"));
                    //ret += FOUR_SPACES "Description:\n";
                    //ret += indent;
                    //ret += EIGHT_SPACES;
                    ori::change_indent(4);
                    if(main_command_override_name_and_description.has_value()) {
                        ori::print(main_command_override_name_and_description->description);
                    }
                    else {
                        ori::print(docs.description);
                    }
                    ori::print("\n\n");
                    ori::change_indent(-8);
                }

                if (verbosity == NAME_DESCRIPTION_AND_ARGS_WITH_ARG_DESCRIPTIONS) {
                    if(positional_doc_strings.size()) {
                        //ret += indent;

                        //ret += FOUR_SPACES "Positionals:\n";
                        ori::change_indent(4);
                        ori::println(iro::effect_string(iro::bold, "Positionals:"));

                        ori::change_indent(4);
                        for (const auto& e: positional_doc_strings) {
                            //ret += EIGHT_SPACES;
                            ori::println(std::format("{}: {}", e.name, e.docs));
                        }

                        ori::println();
                        ori::change_indent(-8);
                    }

                    if(docs.variadic) {
                        //ret += indent;
                        //ret += FOUR_SPACES "Variadic:\n";
                        ori::change_indent(4);
                        ori::println(iro::effect_string(iro::bold, "Variadic"));
                        ori::change_indent(4);
                        //ret += EIGHT_SPACES;
                        ori::print(std::format("{}: {}\n\n", variadic_str, docs.variadic->documentation));
                        /*ori::print(variadic_str);
                        ori::print(": ");
                        ori::print(docs.variadic->documentation);
                        ori::print("\n\n");*/

                        ori::change_indent(-8);
                    }

                    if(flag_doc_strings.size()) {
                        //ret += FOUR_SPACES "Flags:\n";
                        ori::change_indent(4);
                        ori::println(iro::effect_string(iro::bold, "Flags:"));

                        ori::change_indent(4);
                        for(const auto& e: flag_doc_strings) {
                            //ret += EIGHT_SPACES;
                            ori::println(std::format("{}: {}", e.name, e.docs));
                        }
                        ori::println();
                        ori::change_indent(-8);
                    }

                    if(option_doc_strings.size()) {
                        //ret += FOUR_SPACES "Options:\n";
                        ori::change_indent(4);
                        ori::println(iro::effect_string(iro::bold, "Options:"));

                        ori::change_indent(4);
                        for(const auto& e: option_doc_strings) {
                            //ret += EIGHT_SPACES;
                            ori::println(std::format("{}: {}", e.name, e.docs));
                        }

                        ori::change_indent(-8);

                        ori::println();
                    }
                }
            }

            if(current_recursion_level+1 <= max_recursion_level) {
                if((docs.subcommands.size() > 1) || !hide_help || ((docs.subcommands.size() > 0) && (!docs.subcommands.contains("help")))) {
                    //ret += FOUR_SPACES "Subcommands:\n";
                    ori::change_indent(4);
                    ori::println(iro::effect_string(iro::bold, "Subcommands:"));
                    std::vector subcommand_name = name;
                    subcommand_name.resize(subcommand_name.size()+1);
                    for(const auto& e : docs.subcommands) {
                        subcommand_name.back() = e;
                        if((!hide_help) || (subcommand_name.back() != "help")) {
                            print_documentation_string_impl(subcommand_name, top_level_verbosity, subcommand_verbosity, max_recursion_level, current_recursion_level + 1, hide_help);

                            if((subcommand_verbosity != NAME_ONLY) &&
                               (subcommand_verbosity != NAME_AND_ARGS) &&
                               (&e != &*(--docs.subcommands.end()))) {
                                    //ret += indent;
                                    //ori::print('\n');
                            }
                        }
                    }

                    ori::change_indent(-4);
                }
            }

            /*#undef FOUR_SPACES
            #undef EIGHT_SPACES*/

            ori::change_indent(-4);
        }
    }

    const command_documentation_t& get_command_docs_from_name(const command_name_t& name) {
        return detail::subcommand_name_to_docs().at(name);
    }

    void default_print_documentation_string_callback(const command_name_t& name,
                                                          const documentation_verbosity& top_level_verbosity, const documentation_verbosity& subcommand_verbosity,
                                                          unsigned max_recursion_level,
                                                          bool hide_help) {
        if((name == command_name_t{"MAIN"}) ||
           (name == command_name_t{})) {
            return detail::print_documentation_string_impl({"MAIN"}, top_level_verbosity, subcommand_verbosity, max_recursion_level, 0, hide_help, detail::name_and_description_t{detail::program_name(), detail::program_description()});
        }
        else {
            return detail::print_documentation_string_impl(name, top_level_verbosity, subcommand_verbosity, max_recursion_level, 0, hide_help);
        }
    }

    namespace detail {
        static constinit print_documentation_string_t print_documentation_string_callback_ = default_print_documentation_string_callback;
    }

    void set_print_documentation_string_callback(print_documentation_string_t callback) {
        detail::print_documentation_string_callback_ = callback;
    }
    print_documentation_string_t get_print_documentation_string_callback() {
        return detail::print_documentation_string_callback_;
    }


    /*std::string default_get_documentation_string_callback(documentation_verbosity verbosity, unsigned max_recursion_level, bool hide_help) {
        return detail::get_documentation_string_impl({"MAIN"}, verbosity, max_recursion_level, 0, detail::name_and_description_t{detail::program_name(), detail::program_description()});
    }*/
}
//end of "documentation.cpp" include

//included from file "../src/subcommand.cpp"

#include <tuple>
#include <sstream>


namespace cppli::detail {

    // construct on first use idiom
    // more info here: https://isocpp.org/wiki/faq/ctors#construct-on-first-use-v2
    std::unordered_map<command_name_t, command_inputs_info_t, command_name_hash_t>& subcommand_name_to_inputs_info() {
        static std::unordered_map<command_name_t, command_inputs_info_t, command_name_hash_t> subcommand_name_to_inputs_info_;

        return subcommand_name_to_inputs_info_;
    }

    std::unordered_map<command_name_t, subcommand_func_t, command_name_hash_t>& subcommand_name_to_func() {
        static std::unordered_map<command_name_t, subcommand_func_t, command_name_hash_t> subcommand_name_to_func_;

        return subcommand_name_to_func_;
    }

    std::unordered_map<command_name_t, command_error_checking_func_t, command_name_hash_t>& subcommand_name_to_error_checking_func() {
        static std::unordered_map<command_name_t, command_error_checking_func_t , command_name_hash_t> subcommand_name_to_error_checking_func_;

        return subcommand_name_to_error_checking_func_;
    }


    bool is_valid_subcommand(command_name_t& parent_command_names, const std::string& arg) {
        if(parent_command_names.size()) {
            auto temp = parent_command_names;
            temp.push_back(arg);
            if(subcommand_name_to_docs().contains(temp)/* || (temp.size() == 0)*/) {
                parent_command_names = std::move(temp);
                return true;
            }
        }
        else if(subcommand_name_to_docs().at({"MAIN"}).subcommands.contains(arg)) {
            parent_command_names.push_back(arg);
            return true;
        }
        return false;
    }

    bool subcommand_takes_flag(const command_name_t& subcommand, const std::string& flag_name) {
        return
            subcommand_name_to_inputs_info().contains(subcommand) &&
            subcommand_name_to_inputs_info().at(subcommand).flags.contains(flag_name);
    }

    bool subcommand_takes_option(const command_name_t& subcommand, const std::string& option_name) {
        return
            subcommand_name_to_inputs_info().contains(subcommand) &&
            subcommand_name_to_inputs_info().at(subcommand).option_argument_is_optional.contains(option_name);
    }

    bool subcommand_option_argument_is_optional(const command_name_t& subcommand, const std::string& option_name) {
        if(subcommand_name_to_inputs_info().at(subcommand).option_argument_is_optional.contains(option_name)) {
            return subcommand_name_to_inputs_info().at(subcommand).option_argument_is_optional.at(option_name);
        }
        return false;
    }

    void error_if_flag_or_option_already_included(const command_t& subcommand, const std::string& flag_or_option) {
        const auto& inputs_info = subcommand_name_to_inputs_info().at(subcommand.name);
        std::optional<std::reference_wrapper<const std::string>> short_name;
        if(inputs_info.flag_or_option_long_name_to_short_name.contains(flag_or_option)) {
            short_name = std::ref(inputs_info.flag_or_option_long_name_to_short_name.at(flag_or_option));
        }

        std::stringstream ss;

        if(inputs_info.flags.contains(flag_or_option)) {
            if(subcommand.inputs.flags.contains(flag_or_option) ||
               (short_name && subcommand.inputs.flags.contains(*short_name))) {

                ss << (subcommand.name == command_name_t{"MAIN"} ? "main command" : "subcommand") << ' ' << to_string(subcommand.name)
                   << " flag --" << flag_or_option << " included multiple times";

                if(short_name && subcommand.inputs.flags.contains(*short_name)) {
                    ss << "(previously included with short name '" << short_name->get() << '\'';
                }

                print_throw_or_do_nothing(FLAG_INCLUDED_MULTIPLE_TIMES, ss.str());
            }
        }
        else {
            if(subcommand.inputs.options_to_values.contains(flag_or_option) ||
                    (short_name && subcommand.inputs.options_to_values.contains(*short_name))) {

                ss << (subcommand.name == command_name_t{"MAIN"} ? "main command" : "subcommand") << ' ' << to_string(subcommand.name)
                   << " option --" << flag_or_option << " included multiple times";

                if(short_name && subcommand.inputs.options_to_values.contains(*short_name)) {
                    ss << "(previously included with short name '" << short_name->get() << '\'';
                }

                print_throw_or_do_nothing(OPTION_INCLUDED_MULTIPLE_TIMES, ss.str(), "\nThe value of the first instance of this option will be used, and all other instances will be ignored\n");
            }
        }
    }

    void error_if_short_flag_or_option_already_included(const command_t& subcommand, const std::string& short_flag_or_option) {
        const auto& inputs_info = subcommand_name_to_inputs_info().at(subcommand.name);
        const std::string& long_name = inputs_info.flag_or_option_short_name_to_long_name.at(short_flag_or_option[0]);

        std::stringstream ss;

        if(inputs_info.flags.contains(short_flag_or_option)) {
            if(subcommand.inputs.flags.contains(short_flag_or_option) ||
               subcommand.inputs.flags.contains(long_name)) {

                ss << (subcommand.name == command_name_t{"MAIN"} ? "main command" : "subcommand") << ' ' << to_string(subcommand.name)
                   << " flag -" << short_flag_or_option << " included multiple times";

                if(subcommand.inputs.flags.contains(long_name)) {
                    ss << " (previously included with long name \"" << long_name << '\"';
                }

                ss << '\n';
                print_throw_or_do_nothing(FLAG_INCLUDED_MULTIPLE_TIMES, ss.str());
            }

        }
        else {
            if(subcommand.inputs.options_to_values.contains(short_flag_or_option) ||
               subcommand.inputs.options_to_values.contains(long_name)) {

                ss << (subcommand.name == command_name_t{"MAIN"} ? "main command" : "subcommand") << ' ' << to_string(subcommand.name)
                   << " option -" << short_flag_or_option << " included multiple times";

                if(subcommand.inputs.options_to_values.contains(long_name)) {
                    ss << "(previously included with long name \"" << long_name << '\"';
                }

                print_throw_or_do_nothing(OPTION_INCLUDED_MULTIPLE_TIMES, ss.str(), "\nThe value of the first instance of this option will be used, and all other instances will be ignored\n");
            }
        }
    }


    bool is_namespace(const command_name_t& subcommand) {
        return subcommand_name_to_docs().at(subcommand).is_namespace;
    }

    bool main_command_is_namespace() {
        return true; // TODO: actual implementation
    }

    static std::string program_name_,
                       program_description_;

    const std::string& program_name() {
        return program_name_;
    }

    const std::string& program_description() {
        return program_description_;
    }

    void set_program_name_and_description(std::string&& name, std::string&& description) {
        program_name_        = std::move(name);
        program_description_ = std::move(description);
    }

    std::string to_string(const command_name_t& name, const char* delimiter) {
        std::string ret;

        if(name.size()) {
            if(name.front() == "MAIN") {
                ret += program_name();
            }
            else {
                ret += name.front();
            }
            if(name.size() > 1) {
                ret += delimiter;
            }
        }
        for(unsigned i = 1; i < name.size()-1; ++i) {
            ret += name[i];
            ret += delimiter;
        }
        if(name.size() > 1) {
            ret += name[name.size()-1];
        }

        return ret;
    }

    std::size_t detail::command_name_hash_t::operator()(const command_name_t& name) const noexcept  {
        std::size_t hash = 0;
        for(const auto& e : name) {
            cppli::detail::hash_combine(hash, e);
        }

        return hash;
    }

    bool command_inputs_t::is_empty() const {
        return ((flags.size() == 0) &&
                (options_to_values.size() == 0) &&
                (positional_args.size() == 0));
    }
}
//end of "subcommand.cpp" include

//included from file "../src/cli_error.cpp"

#include <iostream>



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
//end of "cli_error.cpp" include

//included from file "../src/command_registration.cpp"
#include <format>



namespace cppli::detail {
    void default_help_callback(const command_context_t& cppli_current_command,

                               const variadic<std::string, string_conversion_t<std::string>, false, "subcommand name", "The name of the subcommand to print help for."
                                                                                                                "If no subcommand is provided, then help is printed for the parent command">&, std::vector<std::string> subcommand_name,

                               const flag<"name-only", "only print subcommand names">&, bool name_only,
                               const flag<"name-and-description", "print subcommand name and description">&, bool name_and_description,
                               const flag<"name-and-args", "print subcommand name and args">&, bool name_and_args,
                               const flag<"name-description-and-args", "print subcommand name, description, and args">&, bool name_description_and_args,
                               const flag<"verbose", "print current command name and description", 'v'>&, bool verbose,
                               const flag<"hide-help", "don't show help when printing subcommands">&, bool hide_help,
                               const flag<"show-help", "do show help when printing subcommands">&, bool show_help,

                               const flag<"subcommands-name-only", "only print subcommand names">&, bool subcommands_name_only,
                               const flag<"subcommands-name-and-description", "print subcommand name and description">&, bool subcommands_name_and_description,
                               const flag<"subcommands-name-and-args", "print subcommand name and args">&, bool subcommands_name_and_args,
                               const flag<"subcommands-name-description-and-args", "print subcommand name, description, and args">&, bool subcommands_name_description_and_args,
                               const flag<"subcommands-verbose", "print subcommand name and description">&, bool subcommands_verbose,

                               const option<unsigned, string_conversion_t<unsigned>, false, "recursion", "how many levels of nested subcommands to print. 0 prints none", "unsigned integer", true, false, 'r'>&, const std::optional<unsigned>& recursion) {

        extern command_name_t last_subcommand_;

        documentation_verbosity top_level_verbosity;
        if(verbose) {
            top_level_verbosity = NAME_DESCRIPTION_AND_ARGS_WITH_ARG_DESCRIPTIONS;
        }
        else if(name_description_and_args) {
            top_level_verbosity = NAME_DESCRIPTION_AND_ARGS;
        }
        else if(name_and_args) {
            top_level_verbosity = NAME_AND_ARGS;
        }
        else if(name_and_description) {
            top_level_verbosity = NAME_AND_DESCRIPTION;
        }
        else if(name_only) {
            top_level_verbosity = NAME_ONLY;
        }
        else {
            top_level_verbosity = default_top_level_help_verbosity;
        }

        documentation_verbosity subcommand_verbosity;
        if(subcommands_verbose) {
            subcommand_verbosity = NAME_DESCRIPTION_AND_ARGS_WITH_ARG_DESCRIPTIONS;
        }
        else if(subcommands_name_description_and_args) {
            subcommand_verbosity = NAME_DESCRIPTION_AND_ARGS;
        }
        else if(subcommands_name_and_args) {
            subcommand_verbosity = NAME_AND_ARGS;
        }
        else if(subcommands_name_and_description) {
            subcommand_verbosity = NAME_AND_DESCRIPTION;
        }
        else if(subcommands_name_only) {
            subcommand_verbosity = NAME_ONLY;
        }
        else {
            subcommand_verbosity = default_subcommand_help_verbosity;
        }

        if(hide_help && show_help) {
            std::cerr << "\nboth hide help and show help were provided. Help will be shown\n";
        }

        if(subcommand_name.size()) {
            subcommand_name.insert(subcommand_name.begin(), "MAIN");
            if(!subcommand_name_to_docs().contains(subcommand_name)) {
                std::cerr << iro::bright_red << iro::effect_string(iro::bold|iro::underline, "Error:") << std::format(R"( "{}" does not refer to a valid command)" "\n", to_string(subcommand_name, " "));
                return;
            }
            get_print_documentation_string_callback()(subcommand_name, top_level_verbosity, subcommand_verbosity, recursion.value_or(default_help_recursion_level), (default_hide_help_status || hide_help) && !show_help);
        }
        else {
            get_print_documentation_string_callback()(last_subcommand_, top_level_verbosity, subcommand_verbosity, recursion.value_or(default_help_recursion_level), (default_hide_help_status || hide_help) && !show_help);
        }
    }
}

//end of "command_registration.cpp" include

//included from file "../src/command_macros.cpp"


namespace cppli::detail {
    command_name_t last_subcommand_;
}
//end of "command_macros.cpp" include

//included from file "../src/run.cpp"

#define UTF_CPP_CPLUSPLUS 202002L // we have to do this because MSVC's __cplusplus is broken and the developer of utfcpp refuses to use _MSC_LANG because he wants to "avoid any compiler-specific code" (???)
                                  // I'm not sure what his reasoning is for that policy
                                  // https://github.com/nemtrif/utfcpp/pull/125

//included from file "../lib/utfcpp/source\utf8.h"
// Copyright 2006 Nemanja Trifunovic

/*
Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/


#ifndef UTF8_FOR_CPP_2675DCD0_9480_4c0c_B92A_CC14C027B731
#define UTF8_FOR_CPP_2675DCD0_9480_4c0c_B92A_CC14C027B731

/*
To control the C++ language version used by the library, you can define UTF_CPP_CPLUSPLUS macro
and set it to one of the values used by the __cplusplus predefined macro.

For instance,
    #define UTF_CPP_CPLUSPLUS 199711L
will cause the UTF-8 CPP library to use only types and language features available in the C++ 98 standard.
Some library features will be disabled.

If you leave UTF_CPP_CPLUSPLUS undefined, it will be internally assigned to __cplusplus.
*/


//included from file "C:\programming\hitoribooru\hitoribooru-cli\lib\cppli\single-header-amalgamation\..\lib\utfcpp\source\utf8/checked.h"
// Copyright 2006-2016 Nemanja Trifunovic

/*
Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/


#ifndef UTF8_FOR_CPP_CHECKED_H_2675DCD0_9480_4c0c_B92A_CC14C027B731
#define UTF8_FOR_CPP_CHECKED_H_2675DCD0_9480_4c0c_B92A_CC14C027B731


//included from file "C:\programming\hitoribooru\hitoribooru-cli\lib\cppli\single-header-amalgamation\..\lib\utfcpp\source\utf8\core.h"
// Copyright 2006 Nemanja Trifunovic

/*
Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/


#ifndef UTF8_FOR_CPP_CORE_H_2675DCD0_9480_4c0c_B92A_CC14C027B731
#define UTF8_FOR_CPP_CORE_H_2675DCD0_9480_4c0c_B92A_CC14C027B731

#include <iterator>
#include <cstring>
#include <string>

// Determine the C++ standard version.
// If the user defines UTF_CPP_CPLUSPLUS, use that.
// Otherwise, trust the unreliable predefined macro __cplusplus

#if !defined UTF_CPP_CPLUSPLUS
    #define UTF_CPP_CPLUSPLUS __cplusplus
#endif

#if UTF_CPP_CPLUSPLUS >= 201103L // C++ 11 or later
    #define UTF_CPP_OVERRIDE override
    #define UTF_CPP_NOEXCEPT noexcept
    #define UTF_CPP_STATIC_ASSERT(condition) static_assert(condition, "UTFCPP static assert");
#else // C++ 98/03
    #define UTF_CPP_OVERRIDE
    #define UTF_CPP_NOEXCEPT throw()
    // Simulate static_assert:
    template <bool Condition> struct StaticAssert {static void assert() {int static_assert_impl[(Condition ? 1 : -1)];} };
    template <> struct StaticAssert<true> {static void assert() {}};
    #define UTF_CPP_STATIC_ASSERT(condition) StaticAssert<condition>::assert();
#endif // C++ 11 or later


namespace utf8
{
// The typedefs for 8-bit, 16-bit and 32-bit code units
#if UTF_CPP_CPLUSPLUS >= 201103L // C++ 11 or later
    #if UTF_CPP_CPLUSPLUS >= 202002L // C++ 20 or later
        typedef char8_t         utfchar8_t;
    #else // C++ 11/14/17
        typedef unsigned char   utfchar8_t;
    #endif
    typedef char16_t        utfchar16_t;
    typedef char32_t        utfchar32_t;
#else // C++ 98/03
    typedef unsigned char   utfchar8_t;
    typedef unsigned short  utfchar16_t;
    typedef unsigned int    utfchar32_t;
#endif // C++ 11 or later

// Helper code - not intended to be directly called by the library users. May be changed at any time
namespace internal
{
    // Unicode constants
    // Leading (high) surrogates: 0xd800 - 0xdbff
    // Trailing (low) surrogates: 0xdc00 - 0xdfff
    const utfchar16_t LEAD_SURROGATE_MIN  = 0xd800u;
    const utfchar16_t LEAD_SURROGATE_MAX  = 0xdbffu;
    const utfchar16_t TRAIL_SURROGATE_MIN = 0xdc00u;
    const utfchar16_t TRAIL_SURROGATE_MAX = 0xdfffu;
    const utfchar16_t LEAD_OFFSET         = 0xd7c0u;       // LEAD_SURROGATE_MIN - (0x10000 >> 10)
    const utfchar32_t SURROGATE_OFFSET    = 0xfca02400u;   // 0x10000u - (LEAD_SURROGATE_MIN << 10) - TRAIL_SURROGATE_MIN

    // Maximum valid value for a Unicode code point
    const utfchar32_t CODE_POINT_MAX      = 0x0010ffffu;

    template<typename octet_type>
    inline utfchar8_t mask8(octet_type oc)
    {
        return static_cast<utfchar8_t>(0xff & oc);
    }
    template<typename u16_type>
    inline utfchar16_t mask16(u16_type oc)
    {
        return static_cast<utfchar16_t>(0xffff & oc);
    }

    template<typename octet_type>
    inline bool is_trail(octet_type oc)
    {
        return ((utf8::internal::mask8(oc) >> 6) == 0x2);
    }

    inline bool is_lead_surrogate(utfchar32_t cp)
    {
        return (cp >= LEAD_SURROGATE_MIN && cp <= LEAD_SURROGATE_MAX);
    }

    inline bool is_trail_surrogate(utfchar32_t cp)
    {
        return (cp >= TRAIL_SURROGATE_MIN && cp <= TRAIL_SURROGATE_MAX);
    }

    inline bool is_surrogate(utfchar32_t cp)
    {
        return (cp >= LEAD_SURROGATE_MIN && cp <= TRAIL_SURROGATE_MAX);
    }

    inline bool is_code_point_valid(utfchar32_t cp)
    {
        return (cp <= CODE_POINT_MAX && !utf8::internal::is_surrogate(cp));
    }

    inline bool is_in_bmp(utfchar32_t cp)
    {
        return cp < utfchar32_t(0x10000);
    }

    template <typename octet_iterator>
    int sequence_length(octet_iterator lead_it)
    {
        const utfchar8_t lead = utf8::internal::mask8(*lead_it);
        if (lead < 0x80)
            return 1;
        else if ((lead >> 5) == 0x6)
            return 2;
        else if ((lead >> 4) == 0xe)
            return 3;
        else if ((lead >> 3) == 0x1e)
            return 4;
        else
            return 0;
    }

    inline bool is_overlong_sequence(utfchar32_t cp, int length)
    {
        if (cp < 0x80) {
            if (length != 1) 
                return true;
        }
        else if (cp < 0x800) {
            if (length != 2) 
                return true;
        }
        else if (cp < 0x10000) {
            if (length != 3) 
                return true;
        }
        return false;
    }

    enum utf_error {UTF8_OK, NOT_ENOUGH_ROOM, INVALID_LEAD, INCOMPLETE_SEQUENCE, OVERLONG_SEQUENCE, INVALID_CODE_POINT};

    /// Helper for get_sequence_x
    template <typename octet_iterator>
    utf_error increase_safely(octet_iterator& it, const octet_iterator end)
    {
        if (++it == end)
            return NOT_ENOUGH_ROOM;

        if (!utf8::internal::is_trail(*it))
            return INCOMPLETE_SEQUENCE;

        return UTF8_OK;
    }

    #define UTF8_CPP_INCREASE_AND_RETURN_ON_ERROR(IT, END) {utf_error ret = increase_safely(IT, END); if (ret != UTF8_OK) return ret;}

    /// get_sequence_x functions decode utf-8 sequences of the length x
    template <typename octet_iterator>
    utf_error get_sequence_1(octet_iterator& it, octet_iterator end, utfchar32_t& code_point)
    {
        if (it == end)
            return NOT_ENOUGH_ROOM;

        code_point = utf8::internal::mask8(*it);

        return UTF8_OK;
    }

    template <typename octet_iterator>
    utf_error get_sequence_2(octet_iterator& it, octet_iterator end, utfchar32_t& code_point)
    {
        if (it == end) 
            return NOT_ENOUGH_ROOM;

        code_point = utf8::internal::mask8(*it);

        UTF8_CPP_INCREASE_AND_RETURN_ON_ERROR(it, end)

        code_point = ((code_point << 6) & 0x7ff) + ((*it) & 0x3f);

        return UTF8_OK;
    }

    template <typename octet_iterator>
    utf_error get_sequence_3(octet_iterator& it, octet_iterator end, utfchar32_t& code_point)
    {
        if (it == end)
            return NOT_ENOUGH_ROOM;
            
        code_point = utf8::internal::mask8(*it);

        UTF8_CPP_INCREASE_AND_RETURN_ON_ERROR(it, end)

        code_point = ((code_point << 12) & 0xffff) + ((utf8::internal::mask8(*it) << 6) & 0xfff);

        UTF8_CPP_INCREASE_AND_RETURN_ON_ERROR(it, end)

        code_point = static_cast<utfchar32_t>(code_point + ((*it) & 0x3f));

        return UTF8_OK;
    }

    template <typename octet_iterator>
    utf_error get_sequence_4(octet_iterator& it, octet_iterator end, utfchar32_t& code_point)
    {
        if (it == end)
           return NOT_ENOUGH_ROOM;

        code_point = utf8::internal::mask8(*it);

        UTF8_CPP_INCREASE_AND_RETURN_ON_ERROR(it, end)

        code_point = ((code_point << 18) & 0x1fffff) + ((utf8::internal::mask8(*it) << 12) & 0x3ffff);

        UTF8_CPP_INCREASE_AND_RETURN_ON_ERROR(it, end)

        code_point = static_cast<utfchar32_t>(code_point + ((utf8::internal::mask8(*it) << 6) & 0xfff));

        UTF8_CPP_INCREASE_AND_RETURN_ON_ERROR(it, end)

        code_point = static_cast<utfchar32_t>(code_point + ((*it) & 0x3f));

        return UTF8_OK;
    }

    #undef UTF8_CPP_INCREASE_AND_RETURN_ON_ERROR

    template <typename octet_iterator>
    utf_error validate_next(octet_iterator& it, octet_iterator end, utfchar32_t& code_point)
    {
        if (it == end)
            return NOT_ENOUGH_ROOM;

        // Save the original value of it so we can go back in case of failure
        // Of course, it does not make much sense with i.e. stream iterators
        octet_iterator original_it = it;

        utfchar32_t cp = 0;
        // Determine the sequence length based on the lead octet
        const int length = utf8::internal::sequence_length(it);

        // Get trail octets and calculate the code point
        utf_error err = UTF8_OK;
        switch (length) {
            case 0:
                return INVALID_LEAD;
            case 1:
                err = utf8::internal::get_sequence_1(it, end, cp);
                break;
            case 2:
                err = utf8::internal::get_sequence_2(it, end, cp);
            break;
            case 3:
                err = utf8::internal::get_sequence_3(it, end, cp);
            break;
            case 4:
                err = utf8::internal::get_sequence_4(it, end, cp);
            break;
        }

        if (err == UTF8_OK) {
            // Decoding succeeded. Now, security checks...
            if (utf8::internal::is_code_point_valid(cp)) {
                if (!utf8::internal::is_overlong_sequence(cp, length)){
                    // Passed! Return here.
                    code_point = cp;
                    ++it;
                    return UTF8_OK;
                }
                else
                    err = OVERLONG_SEQUENCE;
            }
            else 
                err = INVALID_CODE_POINT;
        }

        // Failure branch - restore the original value of the iterator
        it = original_it;
        return err;
    }

    template <typename octet_iterator>
    inline utf_error validate_next(octet_iterator& it, octet_iterator end) {
        utfchar32_t ignored;
        return utf8::internal::validate_next(it, end, ignored);
    }

    template <typename word_iterator>
    utf_error validate_next16(word_iterator& it, word_iterator end, utfchar32_t& code_point)
    {
        // Make sure the iterator dereferences a large enough type
        typedef typename std::iterator_traits<word_iterator>::value_type word_type;
        UTF_CPP_STATIC_ASSERT(sizeof(word_type) >= sizeof(utfchar16_t));
        // Check the edge case:
        if (it == end)
            return NOT_ENOUGH_ROOM;
        // Save the original value of it so we can go back in case of failure
        // Of course, it does not make much sense with i.e. stream iterators
        word_iterator original_it = it;

        utf_error err = UTF8_OK;

        const utfchar16_t first_word = *it++;
        if (!is_surrogate(first_word)) {
            code_point = first_word;
            return UTF8_OK;
        }
        else {
            if (it == end)
                err = NOT_ENOUGH_ROOM;
            else if (is_lead_surrogate(first_word)) {
                const utfchar16_t second_word = *it++;
                if (is_trail_surrogate(second_word)) {
                    code_point = static_cast<utfchar32_t>(first_word << 10) + second_word + SURROGATE_OFFSET;
                    return UTF8_OK;
                } else 
                    err = INCOMPLETE_SEQUENCE; 
                
            } else {
                err = INVALID_LEAD;               
            }
        }
        // error branch
        it = original_it;
        return err;
    }

    // Internal implementation of both checked and unchecked append() function
    // This function will be invoked by the overloads below, as they will know
    // the octet_type.
    template <typename octet_iterator, typename octet_type>
    octet_iterator append(utfchar32_t cp, octet_iterator result) {
        if (cp < 0x80)                        // one octet
            *(result++) = static_cast<octet_type>(cp);
        else if (cp < 0x800) {                // two octets
            *(result++) = static_cast<octet_type>((cp >> 6)          | 0xc0);
            *(result++) = static_cast<octet_type>((cp & 0x3f)        | 0x80);
        }
        else if (cp < 0x10000) {              // three octets
            *(result++) = static_cast<octet_type>((cp >> 12)         | 0xe0);
            *(result++) = static_cast<octet_type>(((cp >> 6) & 0x3f) | 0x80);
            *(result++) = static_cast<octet_type>((cp & 0x3f)        | 0x80);
        }
        else {                                // four octets
            *(result++) = static_cast<octet_type>((cp >> 18)         | 0xf0);
            *(result++) = static_cast<octet_type>(((cp >> 12) & 0x3f)| 0x80);
            *(result++) = static_cast<octet_type>(((cp >> 6) & 0x3f) | 0x80);
            *(result++) = static_cast<octet_type>((cp & 0x3f)        | 0x80);
        }
        return result;
    }
    
    // One of the following overloads will be invoked from the API calls

    // A simple (but dangerous) case: the caller appends byte(s) to a char array
    inline char* append(utfchar32_t cp, char* result) {
        return append<char*, char>(cp, result);
    }

    // Hopefully, most common case: the caller uses back_inserter
    // i.e. append(cp, std::back_inserter(str));
    template<typename container_type>
    std::back_insert_iterator<container_type> append
            (utfchar32_t cp, std::back_insert_iterator<container_type> result) {
        return append<std::back_insert_iterator<container_type>,
            typename container_type::value_type>(cp, result);
    }

    // The caller uses some other kind of output operator - not covered above
    // Note that in this case we are not able to determine octet_type
    // so we assume it's utfchar8_t; that can cause a conversion warning if we are wrong.
    template <typename octet_iterator>
    octet_iterator append(utfchar32_t cp, octet_iterator result) {
        return append<octet_iterator, utfchar8_t>(cp, result);
    }

    // Internal implementation of both checked and unchecked append16() function
    // This function will be invoked by the overloads below, as they will know
    // the word_type.
    template <typename word_iterator, typename word_type>
    word_iterator append16(utfchar32_t cp, word_iterator result) {
        UTF_CPP_STATIC_ASSERT(sizeof(word_type) >= sizeof(utfchar16_t));
        if (is_in_bmp(cp))
            *(result++) = static_cast<word_type>(cp);
        else {
            // Code points from the supplementary planes are encoded via surrogate pairs
            *(result++) = static_cast<word_type>(LEAD_OFFSET + (cp >> 10));
            *(result++) = static_cast<word_type>(TRAIL_SURROGATE_MIN + (cp & 0x3FF));
        }
        return result;
    }

    // Hopefully, most common case: the caller uses back_inserter
    // i.e. append16(cp, std::back_inserter(str));
    template<typename container_type>
    std::back_insert_iterator<container_type> append16
            (utfchar32_t cp, std::back_insert_iterator<container_type> result) {
        return append16<std::back_insert_iterator<container_type>,
            typename container_type::value_type>(cp, result);
    }

    // The caller uses some other kind of output operator - not covered above
    // Note that in this case we are not able to determine word_type
    // so we assume it's utfchar16_t; that can cause a conversion warning if we are wrong.
    template <typename word_iterator>
    word_iterator append16(utfchar32_t cp, word_iterator result) {
        return append16<word_iterator, utfchar16_t>(cp, result);
    }

} // namespace internal

    /// The library API - functions intended to be called by the users

    // Byte order mark
    const utfchar8_t bom[] = {0xef, 0xbb, 0xbf};

    template <typename octet_iterator>
    octet_iterator find_invalid(octet_iterator start, octet_iterator end)
    {
        octet_iterator result = start;
        while (result != end) {
            utf8::internal::utf_error err_code = utf8::internal::validate_next(result, end);
            if (err_code != internal::UTF8_OK)
                return result;
        }
        return result;
    }

    inline const char* find_invalid(const char* str)
    {
        const char* end = str + std::strlen(str);
        return find_invalid(str, end); 
    }

    inline std::size_t find_invalid(const std::string& s)
    {
        std::string::const_iterator invalid = find_invalid(s.begin(), s.end());
        return (invalid == s.end()) ? std::string::npos : static_cast<std::size_t>(invalid - s.begin());
    }

    template <typename octet_iterator>
    inline bool is_valid(octet_iterator start, octet_iterator end)
    {
        return (utf8::find_invalid(start, end) == end);
    }

    inline bool is_valid(const char* str)
    {
        return (*(utf8::find_invalid(str)) == '\0');
    }

    inline bool is_valid(const std::string& s)
    {
        return is_valid(s.begin(), s.end());
    }



    template <typename octet_iterator>
    inline bool starts_with_bom (octet_iterator it, octet_iterator end)
    {
        return (
            ((it != end) && (utf8::internal::mask8(*it++)) == bom[0]) &&
            ((it != end) && (utf8::internal::mask8(*it++)) == bom[1]) &&
            ((it != end) && (utf8::internal::mask8(*it))   == bom[2])
           );
    }

    inline bool starts_with_bom(const std::string& s)
    {
        return starts_with_bom(s.begin(), s.end());
    } 
} // namespace utf8

#endif // header guard



//end of "core.h" include
#include <stdexcept>

namespace utf8
{
    // Base for the exceptions that may be thrown from the library
    class exception : public ::std::exception {
    };

    // Exceptions that may be thrown from the library functions.
    class invalid_code_point : public exception {
        utfchar32_t cp;
    public:
        invalid_code_point(utfchar32_t codepoint) : cp(codepoint) {}
        virtual const char* what() const UTF_CPP_NOEXCEPT UTF_CPP_OVERRIDE { return "Invalid code point"; }
        utfchar32_t code_point() const {return cp;}
    };

    class invalid_utf8 : public exception {
        utfchar8_t u8;
    public:
        invalid_utf8 (utfchar8_t u) : u8(u) {}
        invalid_utf8 (char c) : u8(static_cast<utfchar8_t>(c)) {}
        virtual const char* what() const UTF_CPP_NOEXCEPT UTF_CPP_OVERRIDE { return "Invalid UTF-8"; }
        utfchar8_t utf8_octet() const {return u8;}
    };

    class invalid_utf16 : public exception {
        utfchar16_t u16;
    public:
        invalid_utf16 (utfchar16_t u) : u16(u) {}
        virtual const char* what() const UTF_CPP_NOEXCEPT UTF_CPP_OVERRIDE { return "Invalid UTF-16"; }
        utfchar16_t utf16_word() const {return u16;}
    };

    class not_enough_room : public exception {
    public:
        virtual const char* what() const UTF_CPP_NOEXCEPT UTF_CPP_OVERRIDE { return "Not enough space"; }
    };

    /// The library API - functions intended to be called by the users

    template <typename octet_iterator>
    octet_iterator append(utfchar32_t cp, octet_iterator result)
    {
        if (!utf8::internal::is_code_point_valid(cp))
            throw invalid_code_point(cp);

        return internal::append(cp, result);
    }

    inline void append(utfchar32_t cp, std::string& s)
    {
        append(cp, std::back_inserter(s));
    }

    template <typename word_iterator>
    word_iterator append16(utfchar32_t cp, word_iterator result)
    {
        if (!utf8::internal::is_code_point_valid(cp))
            throw invalid_code_point(cp);

        return internal::append16(cp, result);
    }

    template <typename octet_iterator, typename output_iterator>
    output_iterator replace_invalid(octet_iterator start, octet_iterator end, output_iterator out, utfchar32_t replacement)
    {
        while (start != end) {
            octet_iterator sequence_start = start;
            internal::utf_error err_code = utf8::internal::validate_next(start, end);
            switch (err_code) {
                case internal::UTF8_OK :
                    for (octet_iterator it = sequence_start; it != start; ++it)
                        *out++ = *it;
                    break;
                case internal::NOT_ENOUGH_ROOM:
                    out = utf8::append (replacement, out);
                    start = end;
                    break;
                case internal::INVALID_LEAD:
                    out = utf8::append (replacement, out);
                    ++start;
                    break;
                case internal::INCOMPLETE_SEQUENCE:
                case internal::OVERLONG_SEQUENCE:
                case internal::INVALID_CODE_POINT:
                    out = utf8::append (replacement, out);
                    ++start;
                    // just one replacement mark for the sequence
                    while (start != end && utf8::internal::is_trail(*start))
                        ++start;
                    break;
            }
        }
        return out;
    }

    template <typename octet_iterator, typename output_iterator>
    inline output_iterator replace_invalid(octet_iterator start, octet_iterator end, output_iterator out)
    {
        static const utfchar32_t replacement_marker = utf8::internal::mask16(0xfffd);
        return utf8::replace_invalid(start, end, out, replacement_marker);
    }

    inline std::string replace_invalid(const std::string& s, utfchar32_t replacement)
    {
        std::string result;
        replace_invalid(s.begin(), s.end(), std::back_inserter(result), replacement);
        return result;
    }

    inline std::string replace_invalid(const std::string& s)
    {
        std::string result;
        replace_invalid(s.begin(), s.end(), std::back_inserter(result));
        return result;
    }

    template <typename octet_iterator>
    utfchar32_t next(octet_iterator& it, octet_iterator end)
    {
        utfchar32_t cp = 0;
        internal::utf_error err_code = utf8::internal::validate_next(it, end, cp);
        switch (err_code) {
            case internal::UTF8_OK :
                break;
            case internal::NOT_ENOUGH_ROOM :
                throw not_enough_room();
            case internal::INVALID_LEAD :
            case internal::INCOMPLETE_SEQUENCE :
            case internal::OVERLONG_SEQUENCE :
                throw invalid_utf8(static_cast<utfchar8_t>(*it));
            case internal::INVALID_CODE_POINT :
                throw invalid_code_point(cp);
        }
        return cp;
    }

    template <typename word_iterator>
    utfchar32_t next16(word_iterator& it, word_iterator end)
    {
        utfchar32_t cp = 0;
        internal::utf_error err_code = utf8::internal::validate_next16(it, end, cp);
        if (err_code == internal::NOT_ENOUGH_ROOM)
            throw not_enough_room();
        return cp;
    }

    template <typename octet_iterator>
    utfchar32_t peek_next(octet_iterator it, octet_iterator end)
    {
        return utf8::next(it, end);
    }

    template <typename octet_iterator>
    utfchar32_t prior(octet_iterator& it, octet_iterator start)
    {
        // can't do much if it == start
        if (it == start)
            throw not_enough_room();

        octet_iterator end = it;
        // Go back until we hit either a lead octet or start
        while (utf8::internal::is_trail(*(--it)))
            if (it == start)
                throw invalid_utf8(*it); // error - no lead byte in the sequence
        return utf8::peek_next(it, end);
    }

    template <typename octet_iterator, typename distance_type>
    void advance (octet_iterator& it, distance_type n, octet_iterator end)
    {
        const distance_type zero(0);
        if (n < zero) {
            // backward
            for (distance_type i = n; i < zero; ++i)
                utf8::prior(it, end);
        } else {
            // forward
            for (distance_type i = zero; i < n; ++i)
                utf8::next(it, end);
        }
    }

    template <typename octet_iterator>
    typename std::iterator_traits<octet_iterator>::difference_type
    distance (octet_iterator first, octet_iterator last)
    {
        typename std::iterator_traits<octet_iterator>::difference_type dist;
        for (dist = 0; first < last; ++dist)
            utf8::next(first, last);
        return dist;
    }

    template <typename u16bit_iterator, typename octet_iterator>
    octet_iterator utf16to8 (u16bit_iterator start, u16bit_iterator end, octet_iterator result)
    {
        while (start != end) {
            utfchar32_t cp = utf8::internal::mask16(*start++);
            // Take care of surrogate pairs first
            if (utf8::internal::is_lead_surrogate(cp)) {
                if (start != end) {
                    const utfchar32_t trail_surrogate = utf8::internal::mask16(*start++);
                    if (utf8::internal::is_trail_surrogate(trail_surrogate))
                        cp = (cp << 10) + trail_surrogate + internal::SURROGATE_OFFSET;
                    else
                        throw invalid_utf16(static_cast<utfchar16_t>(trail_surrogate));
                }
                else
                    throw invalid_utf16(static_cast<utfchar16_t>(cp));

            }
            // Lone trail surrogate
            else if (utf8::internal::is_trail_surrogate(cp))
                throw invalid_utf16(static_cast<utfchar16_t>(cp));

            result = utf8::append(cp, result);
        }
        return result;
    }

    template <typename u16bit_iterator, typename octet_iterator>
    u16bit_iterator utf8to16 (octet_iterator start, octet_iterator end, u16bit_iterator result)
    {
        while (start < end) {
            const utfchar32_t cp = utf8::next(start, end);
            if (cp > 0xffff) { //make a surrogate pair
                *result++ = static_cast<utfchar16_t>((cp >> 10)   + internal::LEAD_OFFSET);
                *result++ = static_cast<utfchar16_t>((cp & 0x3ff) + internal::TRAIL_SURROGATE_MIN);
            }
            else
                *result++ = static_cast<utfchar16_t>(cp);
        }
        return result;
    }

    template <typename octet_iterator, typename u32bit_iterator>
    octet_iterator utf32to8 (u32bit_iterator start, u32bit_iterator end, octet_iterator result)
    {
        while (start != end)
            result = utf8::append(*(start++), result);

        return result;
    }

    template <typename octet_iterator, typename u32bit_iterator>
    u32bit_iterator utf8to32 (octet_iterator start, octet_iterator end, u32bit_iterator result)
    {
        while (start < end)
            (*result++) = utf8::next(start, end);

        return result;
    }

    // The iterator class
    template <typename octet_iterator>
    class iterator {
      octet_iterator it;
      octet_iterator range_start;
      octet_iterator range_end;
      public:
      typedef utfchar32_t value_type;
      typedef utfchar32_t* pointer;
      typedef utfchar32_t& reference;
      typedef std::ptrdiff_t difference_type;
      typedef std::bidirectional_iterator_tag iterator_category;
      iterator () {}
      explicit iterator (const octet_iterator& octet_it,
                         const octet_iterator& rangestart,
                         const octet_iterator& rangeend) :
               it(octet_it), range_start(rangestart), range_end(rangeend)
      {
          if (it < range_start || it > range_end)
              throw std::out_of_range("Invalid utf-8 iterator position");
      }
      // the default "big three" are OK
      octet_iterator base () const { return it; }
      utfchar32_t operator * () const
      {
          octet_iterator temp = it;
          return utf8::next(temp, range_end);
      }
      bool operator == (const iterator& rhs) const
      {
          if (range_start != rhs.range_start || range_end != rhs.range_end)
              throw std::logic_error("Comparing utf-8 iterators defined with different ranges");
          return (it == rhs.it);
      }
      bool operator != (const iterator& rhs) const
      {
          return !(operator == (rhs));
      }
      iterator& operator ++ ()
      {
          utf8::next(it, range_end);
          return *this;
      }
      iterator operator ++ (int)
      {
          iterator temp = *this;
          utf8::next(it, range_end);
          return temp;
      }
      iterator& operator -- ()
      {
          utf8::prior(it, range_start);
          return *this;
      }
      iterator operator -- (int)
      {
          iterator temp = *this;
          utf8::prior(it, range_start);
          return temp;
      }
    }; // class iterator

} // namespace utf8

#if UTF_CPP_CPLUSPLUS >= 202002L // C++ 20 or later

//included from file "C:\programming\hitoribooru\hitoribooru-cli\lib\cppli\single-header-amalgamation\..\lib\utfcpp\source\utf8\cpp20.h"
// Copyright 2022 Nemanja Trifunovic

/*
Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/


#ifndef UTF8_FOR_CPP_207e906c01_03a3_4daf_b420_ea7ea952b3c9
#define UTF8_FOR_CPP_207e906c01_03a3_4daf_b420_ea7ea952b3c9


//included from file "C:\programming\hitoribooru\hitoribooru-cli\lib\cppli\single-header-amalgamation\..\lib\utfcpp\source\utf8\cpp17.h"
// Copyright 2018 Nemanja Trifunovic

/*
Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/


#ifndef UTF8_FOR_CPP_7e906c01_03a3_4daf_b420_ea7ea952b3c9
#define UTF8_FOR_CPP_7e906c01_03a3_4daf_b420_ea7ea952b3c9


//included from file "C:\programming\hitoribooru\hitoribooru-cli\lib\cppli\single-header-amalgamation\..\lib\utfcpp\source\utf8\cpp11.h"
// Copyright 2018 Nemanja Trifunovic

/*
Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/


#ifndef UTF8_FOR_CPP_a184c22c_d012_11e8_a8d5_f2801f1b9fd1
#define UTF8_FOR_CPP_a184c22c_d012_11e8_a8d5_f2801f1b9fd1


namespace utf8
{
    inline void append16(utfchar32_t cp, std::u16string& s)
    {
        append16(cp, std::back_inserter(s));
    }

    inline std::string utf16to8(const std::u16string& s)
    {
        std::string result;
        utf16to8(s.begin(), s.end(), std::back_inserter(result));
        return result;
    }

    inline std::u16string utf8to16(const std::string& s)
    {
        std::u16string result;
        utf8to16(s.begin(), s.end(), std::back_inserter(result));
        return result;
    }

    inline std::string utf32to8(const std::u32string& s)
    {
        std::string result;
        utf32to8(s.begin(), s.end(), std::back_inserter(result));
        return result;
    }

    inline std::u32string utf8to32(const std::string& s)
    {
        std::u32string result;
        utf8to32(s.begin(), s.end(), std::back_inserter(result));
        return result;
    }
} // namespace utf8

#endif // header guard


//end of "cpp11.h" include

namespace utf8
{
    inline std::string utf16to8(std::u16string_view s)
    {
        std::string result;
        utf16to8(s.begin(), s.end(), std::back_inserter(result));
        return result;
    }

    inline std::u16string utf8to16(std::string_view s)
    {
        std::u16string result;
        utf8to16(s.begin(), s.end(), std::back_inserter(result));
        return result;
    }

    inline std::string utf32to8(std::u32string_view s)
    {
        std::string result;
        utf32to8(s.begin(), s.end(), std::back_inserter(result));
        return result;
    }

    inline std::u32string utf8to32(std::string_view s)
    {
        std::u32string result;
        utf8to32(s.begin(), s.end(), std::back_inserter(result));
        return result;
    }

    inline std::size_t find_invalid(std::string_view s)
    {
        std::string_view::const_iterator invalid = find_invalid(s.begin(), s.end());
        return (invalid == s.end()) ? std::string_view::npos : static_cast<std::size_t>(invalid - s.begin());
    }

    inline bool is_valid(std::string_view s)
    {
        return is_valid(s.begin(), s.end());
    }

    inline std::string replace_invalid(std::string_view s, char32_t replacement)
    {
        std::string result;
        replace_invalid(s.begin(), s.end(), std::back_inserter(result), replacement);
        return result;
    }

    inline std::string replace_invalid(std::string_view s)
    {
        std::string result;
        replace_invalid(s.begin(), s.end(), std::back_inserter(result));
        return result;
    }

    inline bool starts_with_bom(std::string_view s)
    {
        return starts_with_bom(s.begin(), s.end());
    }
 
} // namespace utf8

#endif // header guard


//end of "cpp17.h" include

namespace utf8
{
    inline std::u8string utf16tou8(const std::u16string& s)
    {
        std::u8string result;
        utf16to8(s.begin(), s.end(), std::back_inserter(result));
        return result;
    }

    inline std::u8string utf16tou8(std::u16string_view s)
    {
        std::u8string result;
        utf16to8(s.begin(), s.end(), std::back_inserter(result));
        return result;
    }

    inline std::u16string utf8to16(const std::u8string& s)
    {
        std::u16string result;
        utf8to16(s.begin(), s.end(), std::back_inserter(result));
        return result;
    }

    inline std::u16string utf8to16(const std::u8string_view& s)
    {
        std::u16string result;
        utf8to16(s.begin(), s.end(), std::back_inserter(result));
        return result;
    }

    inline std::u8string utf32tou8(const std::u32string& s)
    {
        std::u8string result;
        utf32to8(s.begin(), s.end(), std::back_inserter(result));
        return result;
    }

    inline std::u8string utf32tou8(const std::u32string_view& s)
    {
        std::u8string result;
        utf32to8(s.begin(), s.end(), std::back_inserter(result));
        return result;
    }

    inline std::u32string utf8to32(const std::u8string& s)
    {
        std::u32string result;
        utf8to32(s.begin(), s.end(), std::back_inserter(result));
        return result;
    }

    inline std::u32string utf8to32(const std::u8string_view& s)
    {
        std::u32string result;
        utf8to32(s.begin(), s.end(), std::back_inserter(result));
        return result;
    }

    inline std::size_t find_invalid(const std::u8string& s)
    {
        std::u8string::const_iterator invalid = find_invalid(s.begin(), s.end());
        return (invalid == s.end()) ? std::string_view::npos : static_cast<std::size_t>(invalid - s.begin());
    }

    inline bool is_valid(const std::u8string& s)
    {
        return is_valid(s.begin(), s.end());
    }

    inline std::u8string replace_invalid(const std::u8string& s, char32_t replacement)
    {
        std::u8string result;
        replace_invalid(s.begin(), s.end(), std::back_inserter(result), replacement);
        return result;
    }

    inline std::u8string replace_invalid(const std::u8string& s)
    {
        std::u8string result;
        replace_invalid(s.begin(), s.end(), std::back_inserter(result));
        return result;
    }

    inline bool starts_with_bom(const std::u8string& s)
    {
        return starts_with_bom(s.begin(), s.end());
    }
 
} // namespace utf8

#endif // header guard


//end of "cpp20.h" include
#elif UTF_CPP_CPLUSPLUS >= 201703L // C++ 17 or later
#elif UTF_CPP_CPLUSPLUS >= 201103L // C++ 11 or later
#endif // C++ 11 or later

#endif //header guard


//end of "utf8/checked.h" include

//included from file "C:\programming\hitoribooru\hitoribooru-cli\lib\cppli\single-header-amalgamation\..\lib\utfcpp\source\utf8/unchecked.h"
// Copyright 2006 Nemanja Trifunovic

/*
Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/


#ifndef UTF8_FOR_CPP_UNCHECKED_H_2675DCD0_9480_4c0c_B92A_CC14C027B731
#define UTF8_FOR_CPP_UNCHECKED_H_2675DCD0_9480_4c0c_B92A_CC14C027B731


namespace utf8
{
    namespace unchecked
    {
        template <typename octet_iterator>
        octet_iterator append(utfchar32_t cp, octet_iterator result)
        {
            return internal::append(cp, result);
        }

        template <typename word_iterator>
        word_iterator append16(utfchar32_t cp, word_iterator result)
        {
            return internal::append16(cp, result);
        }

        template <typename octet_iterator, typename output_iterator>
        output_iterator replace_invalid(octet_iterator start, octet_iterator end, output_iterator out, utfchar32_t replacement)
        {
            while (start != end) {
                octet_iterator sequence_start = start;
                internal::utf_error err_code = utf8::internal::validate_next(start, end);
                switch (err_code) {
                    case internal::UTF8_OK :
                        for (octet_iterator it = sequence_start; it != start; ++it)
                            *out++ = *it;
                        break;
                    case internal::NOT_ENOUGH_ROOM:
                        out = utf8::unchecked::append(replacement, out);
                        start = end;
                        break;
                    case internal::INVALID_LEAD:
                        out = utf8::unchecked::append(replacement, out);
                        ++start;
                        break;
                    case internal::INCOMPLETE_SEQUENCE:
                    case internal::OVERLONG_SEQUENCE:
                    case internal::INVALID_CODE_POINT:
                        out = utf8::unchecked::append(replacement, out);
                        ++start;
                        // just one replacement mark for the sequence
                        while (start != end && utf8::internal::is_trail(*start))
                            ++start;
                        break;
                }
            }
            return out;
        }

        template <typename octet_iterator, typename output_iterator>
        inline output_iterator replace_invalid(octet_iterator start, octet_iterator end, output_iterator out)
        {
            static const utfchar32_t replacement_marker = utf8::internal::mask16(0xfffd);
            return utf8::unchecked::replace_invalid(start, end, out, replacement_marker);
        }

        inline std::string replace_invalid(const std::string& s, utfchar32_t replacement)
        {
            std::string result;
            replace_invalid(s.begin(), s.end(), std::back_inserter(result), replacement);
            return result;
        }

        inline std::string replace_invalid(const std::string& s)
        {
            std::string result;
            replace_invalid(s.begin(), s.end(), std::back_inserter(result));
            return result;
        }

        template <typename octet_iterator>
        utfchar32_t next(octet_iterator& it)
        {
            utfchar32_t cp = utf8::internal::mask8(*it);
            switch (utf8::internal::sequence_length(it)) {
                case 1:
                    break;
                case 2:
                    it++;
                    cp = ((cp << 6) & 0x7ff) + ((*it) & 0x3f);
                    break;
                case 3:
                    ++it; 
                    cp = ((cp << 12) & 0xffff) + ((utf8::internal::mask8(*it) << 6) & 0xfff);
                    ++it;
                    cp = static_cast<utfchar32_t>(cp + ((*it) & 0x3f));
                    break;
                case 4:
                    ++it;
                    cp = ((cp << 18) & 0x1fffff) + ((utf8::internal::mask8(*it) << 12) & 0x3ffff);                
                    ++it;
                    cp = static_cast<utfchar32_t>(cp + ((utf8::internal::mask8(*it) << 6) & 0xfff));
                    ++it;
                    cp = static_cast<utfchar32_t>(cp + ((*it) & 0x3f)); 
                    break;
            }
            ++it;
            return cp;
        }

        template <typename octet_iterator>
        utfchar32_t peek_next(octet_iterator it)
        {
            return utf8::unchecked::next(it);
        }

        template <typename word_iterator>
        utfchar32_t next16(word_iterator& it)
        {
            utfchar32_t cp = utf8::internal::mask16(*it++);
            if (utf8::internal::is_lead_surrogate(cp))
                return (cp << 10) + *it++ + utf8::internal::SURROGATE_OFFSET;
            return cp;
        }

        template <typename octet_iterator>
        utfchar32_t prior(octet_iterator& it)
        {
            while (utf8::internal::is_trail(*(--it))) ;
            octet_iterator temp = it;
            return utf8::unchecked::next(temp);
        }

        template <typename octet_iterator, typename distance_type>
        void advance(octet_iterator& it, distance_type n)
        {
            const distance_type zero(0);
            if (n < zero) {
                // backward
                for (distance_type i = n; i < zero; ++i)
                    utf8::unchecked::prior(it);
            } else {
                // forward
                for (distance_type i = zero; i < n; ++i)
                    utf8::unchecked::next(it);
            }
        }

        template <typename octet_iterator>
        typename std::iterator_traits<octet_iterator>::difference_type
        distance(octet_iterator first, octet_iterator last)
        {
            typename std::iterator_traits<octet_iterator>::difference_type dist;
            for (dist = 0; first < last; ++dist) 
                utf8::unchecked::next(first);
            return dist;
        }

        template <typename u16bit_iterator, typename octet_iterator>
        octet_iterator utf16to8(u16bit_iterator start, u16bit_iterator end, octet_iterator result)
        {
            while (start != end) {
                utfchar32_t cp = utf8::internal::mask16(*start++);
                // Take care of surrogate pairs first
                if (utf8::internal::is_lead_surrogate(cp)) {
                    if (start == end)
                        return result;
                    utfchar32_t trail_surrogate = utf8::internal::mask16(*start++);
                    cp = (cp << 10) + trail_surrogate + internal::SURROGATE_OFFSET;
                }
                result = utf8::unchecked::append(cp, result);
            }
            return result;
        }

        template <typename u16bit_iterator, typename octet_iterator>
        u16bit_iterator utf8to16(octet_iterator start, octet_iterator end, u16bit_iterator result)
        {
            while (start < end) {
                utfchar32_t cp = utf8::unchecked::next(start);
                if (cp > 0xffff) { //make a surrogate pair
                    *result++ = static_cast<utfchar16_t>((cp >> 10)   + internal::LEAD_OFFSET);
                    *result++ = static_cast<utfchar16_t>((cp & 0x3ff) + internal::TRAIL_SURROGATE_MIN);
                }
                else
                    *result++ = static_cast<utfchar16_t>(cp);
            }
            return result;
        }

        template <typename octet_iterator, typename u32bit_iterator>
        octet_iterator utf32to8(u32bit_iterator start, u32bit_iterator end, octet_iterator result)
        {
            while (start != end)
                result = utf8::unchecked::append(*(start++), result);

            return result;
        }

        template <typename octet_iterator, typename u32bit_iterator>
        u32bit_iterator utf8to32(octet_iterator start, octet_iterator end, u32bit_iterator result)
        {
            while (start < end)
                (*result++) = utf8::unchecked::next(start);

            return result;
        }

        // The iterator class
        template <typename octet_iterator>
          class iterator {
            octet_iterator it;
            public:
            typedef utfchar32_t value_type;
            typedef utfchar32_t* pointer;
            typedef utfchar32_t& reference;
            typedef std::ptrdiff_t difference_type;
            typedef std::bidirectional_iterator_tag iterator_category;
            iterator () {}
            explicit iterator (const octet_iterator& octet_it): it(octet_it) {}
            // the default "big three" are OK
            octet_iterator base () const { return it; }
            utfchar32_t operator * () const
            {
                octet_iterator temp = it;
                return utf8::unchecked::next(temp);
            }
            bool operator == (const iterator& rhs) const 
            { 
                return (it == rhs.it);
            }
            bool operator != (const iterator& rhs) const
            {
                return !(operator == (rhs));
            }
            iterator& operator ++ () 
            {
                ::std::advance(it, utf8::internal::sequence_length(it));
                return *this;
            }
            iterator operator ++ (int)
            {
                iterator temp = *this;
                ::std::advance(it, utf8::internal::sequence_length(it));
                return temp;
            }  
            iterator& operator -- ()
            {
                utf8::unchecked::prior(it);
                return *this;
            }
            iterator operator -- (int)
            {
                iterator temp = *this;
                utf8::unchecked::prior(it);
                return temp;
            }
          }; // class iterator

    } // namespace utf8::unchecked
} // namespace utf8 


#endif // header guard


//end of "utf8/unchecked.h" include

#endif // header guard

//end of "utf8.h" include


namespace cppli {
    namespace detail {
        extern command_name_t last_subcommand_;

        int run_impl_(const std::vector<std::string>& arg_vec) {
            try {
                {
                    command_name_t main_help = {"MAIN", "help"};
                    if (!subcommand_name_to_func().contains(main_help)) {
                        register_command<default_help_callback>(main_help, "print help for this command", true);
                    }
                }

                auto parse_ret = detail::parse(arg_vec);

                if (parse_ret.help_command_index && !parse_ret.printed_help) {
                    // help command is special. Skip all other execution if it is encountered
                    const auto& help_command = parse_ret.subcommands[*parse_ret.help_command_index];
                    last_subcommand_ = parse_ret.subcommands[parse_ret.subcommands.size() - 2].name;
                    (detail::subcommand_name_to_func()[help_command.name])(help_command, {true});
                }                                                                         // help is always leaf
                else if (!parse_ret.printed_help) {
                    const auto& commands_vec = parse_ret.subcommands;

                    #ifdef CPPLI_FULL_ERROR_CHECKING_BEFORE_RUN
                        for(const auto& command : commands_vec) { // throws if any errors would occur calling the given commands, without actually calling them
                            if(detail::subcommand_name_to_error_checking_func().contains(command.name)) {
                                (detail::subcommand_name_to_error_checking_func()[command.name])(command);
                            }
                        }
                    #endif


                    bool runnable_command_found = false;

                    if (!detail::subcommand_name_to_docs()[{"MAIN"}].is_namespace) {
                        (detail::subcommand_name_to_func()[{"MAIN"}])(commands_vec[0], {(commands_vec.size() < 2)});
                        runnable_command_found = true;
                        detail::last_subcommand_ = {};
                    }

                    for (unsigned i = 1; i < commands_vec.size(); ++i) {
                        detail::last_subcommand_ = commands_vec[i - 1].name;
                        if ((detail::subcommand_name_to_func().contains(commands_vec[i].name))) {
                            runnable_command_found = true;
                            (detail::subcommand_name_to_func()[commands_vec[i].name])(commands_vec[i], {(i == commands_vec.size() - 1)});
                        }
                    }

                    if (!runnable_command_found) {
                        throw cli_error("The input did not form any runnable commands", NO_RUNNABLE_COMMAND_FOUND);
                    }
                }
            }
            catch(cppli::error& e) {
                print_fancy_error(std::cerr, e.what());
                return e.error_code();
            }

            return 0;
        }

        int run_impl_(int argc, const char* const* argv) {
            return run_impl_(argv_to_arg_vec(argc, argv));
        }
    }

    #ifdef _WIN32
        std::vector<std::string> wmain_utf16_argv_to_utf8(int argc, wchar_t** argv) {
            static_assert(sizeof(wchar_t) == sizeof(char16_t), "paranoia size check. This assert should never fail");
            static_assert(sizeof(wchar_t) * CHAR_BIT == 16,    "paranoia size check. This assert should never fail");

            std::vector<std::string> ret;
            ret.resize(argc);
            for (unsigned i = 0; i < argc; ++i) {
                /// two memcpy calls to convert utf16 to utf8 IN ADDITION to using functions from a utility library. Absolute state of C++
                /// if there is a better way to do this, please let me know (if std::start_lifetime_as ever gets implemented, it could be used to reduce the number of copies)
                auto wstr = std::wstring_view(argv[i]);

                std::u16string u16string;
                u16string.resize(wstr.size());
                std::memcpy(u16string.data(), wstr.data(), sizeof(wchar_t) * wstr.size());

                std::u8string u8string = utf8::utf16tou8(u16string);
                ret[i].resize(u8string.size());
                std::memcpy(ret[i].data(), u8string.data(), sizeof(char8_t) * u8string.size());
            }

            return ret; //nrvo
        }
    #endif
}


//end of "run.cpp" include
#endif



