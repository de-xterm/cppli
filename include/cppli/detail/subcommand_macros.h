#pragma once

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