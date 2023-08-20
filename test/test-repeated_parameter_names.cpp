
#include "catch_test_macros.hpp"

#ifdef CPPLI_SINGLE_HEADER
    #include "cppli.h"
#else
    #include "cppli/command_macros.h"
#endif

using namespace cppli::detail;

void good1(const flag<"foo", "foo flag", 'f'>&, const bool& f){}

void good2(const flag<"foo", "foo flag", 'f'>&, const bool& f,
           const flag<"bar", "foo flag", 'b'>&, const bool& b){}

void good3(const flag<"foo", "foo flag", 'f'>&, const bool& f,
           const flag<"bar", "foo flag", 'b'>&, const bool& b,
           const flag<"baz", "foo flag", 'z'>&, const bool& z){}

void error1(const flag<"foo", "foo flag", 'f'>&, const bool& f,
            const flag<"bar", "foo flag", 'f'>&, const bool& b){}

void error2(const flag<"foo", "foo flag", 'f'>&, const bool& f,
            const flag<"bar", "foo flag", 'b'>&, const bool& b,
            const flag<"baz", "foo flag", 'f'>&, const bool& z){}

void error3(const flag<"foo", "foo flag", 'f'>&, const bool& f,
            const flag<"bar", "foo flag", 'b'>&, const bool& b,
            const flag<"baz", "foo flag", 'b'>&, const bool& z){}

template<typename return_t, typename...arg_ts>
void deduce_args_and_check_for_repeated_short_name(bool expected_value, return_t(*)(arg_ts...)) {
    REQUIRE(cppli::detail::no_repeated_short_names_v<arg_ts...> == expected_value);
}

TEST_CASE("no_repeated_short_names_v works") {
    SECTION("invalid callback fail") {
        deduce_args_and_check_for_repeated_short_name(false, error1);
        deduce_args_and_check_for_repeated_short_name(false, error2);
        deduce_args_and_check_for_repeated_short_name(false, error3);
    }

    SECTION("valid callbacks succeed") {
        deduce_args_and_check_for_repeated_short_name(true,  good1);
        deduce_args_and_check_for_repeated_short_name(true,  good2);
        deduce_args_and_check_for_repeated_short_name(true,  good3);
    }
}