#pragma once

#include "subcommand.h"
#include "documentation.h"
#include "user_error.h"
#include "configuration.h"
#include "template_utils.h"
#include "process_argument.h"
#include "argument_processing_checks.h"

namespace cppli::detail {
    template<typename func_t, auto func, typename index_seq>
    struct call_func_wrapper_impl_t;

    template<typename return_t, typename...arg_ts, std::size_t...indices, auto func>
    struct call_func_wrapper_impl_t<return_t(*)(const command_context_t&, arg_ts...), func, std::integer_sequence<std::size_t, indices...>> {
        static void call_func(const subcommand_t& subcommand, const command_context_t& command_context) {
            func(command_context, process_argument<std::remove_cvref_t<get_type_from_index_in_pack<indices, arg_ts...>>, std::remove_cvref_t<get_type_from_index_in_pack<(indices)-1, arg_ts...>>>(count_positionals_before_index_v<indices, arg_ts...>, subcommand)...);
        }

        #ifdef CPPLI_FULL_ERROR_CHECKING_BEFORE_RUN
            static void check_for_errors(const subcommand_t& subcommand) {
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

        static void call_func(const subcommand_t& subcommand, const command_context_t& command_context) {
            constexpr auto variadic_count = count_variadics<std::remove_cvref_t<arg_ts>...>();
            static_assert(variadic_count < 2, "A command can only have 0 or 1 variadics (two or more were found)");

            call_func_wrapper_impl_t<decltype(func), func, std::make_index_sequence<sizeof...(arg_ts)>>::call_func(subcommand, command_context);
        }

        #ifdef CPPLI_FULL_ERROR_CHECKING_BEFORE_RUN
            static void check_for_errors(const subcommand_t& subcommand) {
                call_func_wrapper_impl_t<decltype(func), func, std::make_index_sequence<sizeof...(arg_ts)>>::check_for_errors(subcommand);
            }
        #endif
    };

    template<auto func>
    struct call_func_wrapper_t<void(*)(const command_context_t&), func> { // we need all this ugly partial specializations so that we can deduce arg_ts and indices
        static void call_func(const subcommand_t& subcommand, const command_context_t& command_context) {
            func(command_context);
        }

        static void check_for_errors(const subcommand_t& subcommand) {
            // do nothing
        }
    };

    template<auto func>
    void call_func(const subcommand_t& command, const command_context_t& command_context) {
        call_func_wrapper_t<decltype(func), func>::call_func(command, command_context);
    }

    #ifdef CPPLI_FULL_ERROR_CHECKING_BEFORE_RUN
        template<auto func>
        void check_for_errors(const subcommand_t& command) {
            call_func_wrapper_t<decltype(func), func>::check_for_errors(command);
        }
    #endif

    template<typename return_t, typename arg_t, typename...arg_ts>                      // don't actually care about func, just need to deduce args
    void generate_input_info_and_docs_impl(subcommand_inputs_info_t& info, subcommand_documentation_t& documentation, return_t(*func)(const command_context_t&, arg_t, arg_ts...) = nullptr) {
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
    void generate_input_info_and_docs(subcommand_inputs_info_t& info, subcommand_documentation_t& documentation, return_t(*func)(const command_context_t&, arg_t, arg_ts...) = nullptr) {
        static_assert(no_repeated_short_names_v<arg_t, arg_ts...>, "multiple parameters cannot have the same short name");
        generate_input_info_and_docs_impl(info, documentation, func);
    }

    template<typename return_t>                 // don't actually care about func, just need to deduce args
    void generate_input_info_and_docs(subcommand_inputs_info_t& info, subcommand_documentation_t& documentation, return_t(*func)(const command_context_t&) = nullptr) {
        // do nothing
    }

    // we don't have command_macros yet, so we have to write the callback signature manually
    void default_help_callback(const command_context_t& cppli_current_command,

                               const variadic<std::string, conversion_t<std::string>, false, "subcommand name", "The name of the subcommand to print help for."
                                                                                                                "If no subcommand is provided, then help is printed for the parent command">&, std::vector<std::string> subcommand_name,

                               const flag<"name-only", "only print subcommand names">&,                                  bool name_only,
                               const flag<"name-and-description", "print subcommand name and description">&,             bool name_and_description,
                               const flag<"name-and-args", "print subcommand name and args">&,                           bool name_and_args,
                               const flag<"name-description-and-args", "print subcommand name, description, and args">&, bool name_description_and_args,
                               const flag<"verbose", "print subcommand name and description", 'v'>&,                     bool verbose,
                               const flag<"hide-help", "don't show help when printing subcommands">&,                    bool hide_help,
                               const flag<"show-help", "do show help when printing subcommands">&,                       bool show_help,

                               const flag<"subcommands-name-only", "only print subcommand names">&,                                  bool subcommands_name_only,
                               const flag<"subcommands-name-and-description", "print subcommand name and description">&,             bool subcommands_name_and_description,
                               const flag<"subcommands-name-and-args", "print subcommand name and args">&,                           bool subcommands_name_and_args,
                               const flag<"subcommands-name-description-and-args", "print subcommand name, description, and args">&, bool subcommands_name_description_and_args,
                               const flag<"subcommands-verbose", "print subcommand name and description">&,                          bool subcommands_verbose,

                               const option<unsigned, conversion_t<unsigned>, false, "recursion", "how many levels of nested subcommands to print. 0 prints none", "unsigned integer", true, false, 'r'>&, const std::optional<unsigned>& recursion);

    template<auto func>
    dummy_t register_command(const subcommand_name_t& name, const char* description, bool is_help = false) {
        subcommand_inputs_info_t   info;
        subcommand_documentation_t docs(name.back(), description);

        subcommand_name_t cumulative_name;
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

        if(name == subcommand_name_t{"MAIN"}) {
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
            subcommand_name_t temp;
            for(const auto& command : name) {
                temp.push_back(command);

                subcommand_name_t temp_plus_help = temp;
                temp_plus_help.push_back("help");
                if(!subcommand_name_to_func().contains(temp)) {
                    register_command<default_help_callback>(temp_plus_help, "print help for this command", true);
                }
            }
        }

        return {};
    }
}
