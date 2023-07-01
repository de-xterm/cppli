#pragma once

namespace cppli::detail {
    struct evaluate_at_file_scope_dummy_t {
        template<typename T>
        evaluate_at_file_scope_dummy_t(T&&) {}
    };

    #define cppli_internal_PARENS ()

    #define cppli_internal_EXPAND(...) cppli_internal_EXPAND2(cppli_internal_EXPAND2(cppli_internal_EXPAND2(cppli_internal_EXPAND2(__VA_ARGS__))))
    /*#define cppli_internal_EXPAND4(...) cppli_internal_EXPAND3(cppli_internal_EXPAND3(cppli_internal_EXPAND3(cppli_internal_EXPAND3(__VA_ARGS__))))
    #define cppli_internal_EXPAND3(...) cppli_internal_EXPAND2(cppli_internal_EXPAND2(cppli_internal_EXPAND2(cppli_internal_EXPAND2(__VA_ARGS__))))*/
    #define cppli_internal_EXPAND2(...) cppli_internal_EXPAND1(cppli_internal_EXPAND1(cppli_internal_EXPAND1(cppli_internal_EXPAND1(__VA_ARGS__))))
    #define cppli_internal_EXPAND1(...) __VA_ARGS__

    #define cppli_internal_FOR_EACH(macro, ...)                                    \
          __VA_OPT__(cppli_internal_EXPAND(cppli_internal_FOR_EACH_HELPER(macro, __VA_ARGS__)))

    #define cppli_internal_FOR_EACH_HELPER(macro, a1, ...)                                    \
          macro(a1)                                                                           \
          __VA_OPT__(cppli_internal_FOR_EACH_AGAIN cppli_internal_PARENS (macro, __VA_ARGS__))

    #define cppli_internal_FOR_EACH_AGAIN() cppli_internal_FOR_EACH_HELPER


    #define cppli_internal_STRINGIFY(S) cppli_internal_STRINGIFY_impl(S)
    #define cppli_internal_STRINGIFY_impl(S) #S

    #define cppli_internal_STRINGIFY_WITH_COMMA(S) cppli_internal_STRINGIFY(S),

    #define ONE_ARG_CAT(arg1) arg1

    #define NOT_ONE_ARG_CAT(...) \
        cppli_internal_EXPAND(cppli_internal_NOT_FINAL_CAT_HELPER(__VA_ARGS__))

    #define cppli_internal_CAT(arg1, ...)                                    \
        __VA_OPT__(NOT_)##ONE_ARG_CAT(arg1 __VA_OPT__(, __VA_ARGS__))

    #define cppli_internal_NOT_FINAL_CAT_HELPER(a1, a2, ...)                         \
          __VA_OPT__(NOT_)##FINAL_CAT_AGAIN cppli_internal_PARENS (a1##_##a2 __VA_OPT__(, __VA_ARGS__))

    #define FINAL_CAT_HELPER(arg) arg

    #define FINAL_CAT_AGAIN() FINAL_CAT_HELPER
    #define NOT_FINAL_CAT_AGAIN() cppli_internal_NOT_FINAL_CAT_HELPER

    #define cppli_internal_UNIQUE_NAME cppli_internal_CAT(name, __COUNTER__)

    #define cppli_internal_EVALUATE_AT_FILE_SCOPE(EXPR)                        \
    namespace {                                                 \
        ::cppli::detail::evaluate_at_file_scope_dummy_t cppli_internal_UNIQUE_NAME = EXPR;      \
    }
}