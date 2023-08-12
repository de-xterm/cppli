#pragma once

#include "subcommand.h"
#include "documentation.h"
#include "user_error.h"
#include "configuration.h"
#include "template_utils.h"

namespace cppli {
namespace detail {
    template<typename T, typename last>
    /*add_const_ref_if_string_t<T>::*/T process_argument(unsigned cumulative_positional_index, const subcommand_t& subcommand) {
        using arg_info_t = argument_info_t<T>; // no need for remove_cvref, we do that before calling process_argument

        std::string main_command_or_subcommand;

        if(subcommand.name == subcommand_name_t{"MAIN"}) {
            main_command_or_subcommand = "main command";
        }
        else {
            main_command_or_subcommand = "subcommand";
        }

        if constexpr(arg_info_t::is_raw_type) {
            std::string canonical_name = last::name.string();

            std::optional<std::string> short_name;

            if constexpr(last::has_short_name) {
                short_name = std::string{last::short_name};
            }

            if constexpr(argument_info_t<last>::is_flag) {
                return subcommand.inputs.flags.contains(last::name.string()) ||
                       (short_name && subcommand.inputs.flags.contains(std::string{last::short_name}));
            }
            else {
                using conversion_t = typename last::conversion_t;

                if constexpr(argument_info_t<last>::is_option) {
                    if constexpr(last::optional) { // implicitly required argument
                        if constexpr(last::argument_optional) { // I believe default_construct_when_empty is implicity true, so no need for && here
                            if(subcommand.inputs.options_to_values.contains(canonical_name)) {
                                if(subcommand.inputs.options_to_values.at(canonical_name).has_value()) {
                                    try {
                                        return conversions::conversion_t<std::optional<typename last::type>>()(*subcommand.inputs.options_to_values.at(canonical_name)); // no need for has_value check here; returning an empty optional is valid
                                    }
                                    catch(user_error& e) {
                                        throw user_error("Error initializing " + main_command_or_subcommand + " \"" + to_string(subcommand.name) + "\" option \"" + canonical_name + "\". Details: " + e.what(), e.error_type());
                                    }
                                }
                                else {
                                    return typename last::type();
                                }
                            }
                            else if(short_name && subcommand.inputs.options_to_values.contains(*short_name)) {
                                if(subcommand.inputs.options_to_values.at(*short_name).has_value()) {
                                    try {
                                        return conversions::conversion_t<std::optional<typename last::type>>()(*subcommand.inputs.options_to_values.at(*short_name));
                                    }
                                    catch (user_error& e) {
                                        throw user_error("Error initializing " + main_command_or_subcommand + " \"" +
                                                         to_string(subcommand.name) + "\" option \"" + *short_name +
                                                         "\" (full name \"" + canonical_name + "\"). Details: " +
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
                        else {
                            if(subcommand.inputs.options_to_values.contains(canonical_name)) {
                                if(!subcommand.inputs.options_to_values.at(canonical_name).has_value()) { // TODO: aren't we doing this check in arg_parsing.cpp too?
                                    throw user_error(main_command_or_subcommand + to_string(subcommand.name) + "\" option \"" + canonical_name + "\" "
                                                     "requires an argument, but one was not provided (expected an argument of type "
                                                     + static_cast<std::string>(conversion_t::type_string.make_lowercase_and_convert_underscores()) + "."
                                                     "Note that this option is optional, so it is valid to omit it entirely, "
                                                     "but the option's argument is required, so if the option is provided, it must come with an argument",
                                                     OPTION_REQUIRED_ARGUMENT_NOT_PROVIDED);
                                }
                                else {
                                    try {
                                        return conversion_t()(*subcommand.inputs.options_to_values.at(canonical_name)); // no need for has_value check here; returning an empty optional is valid
                                    }
                                    catch(user_error& e) {
                                        throw user_error("Error initializing " + main_command_or_subcommand + " \"" + to_string(subcommand.name) + "\" option \"" + canonical_name + "\". Details: " + e.what(), e.error_type());
                                    }
                                }
                            }
                            else if(short_name && subcommand.inputs.options_to_values.contains(*short_name)) { // TODO: evaluating short_name could use if constexpr
                                if(!subcommand.inputs.options_to_values.at(*short_name).has_value()) {
                                    throw user_error(main_command_or_subcommand + to_string(subcommand.name) + "\" option \"" + *short_name + "\" (full name \"" + canonical_name + "\") "
                                                     "requires an argument, but one was not provided (expected an argument of type "
                                                     + conversion_t::type_string.string() + "."
                                                     "Note that this option is optional, so it is valid to omit it entirely, "
                                                     "but the option's argument is required, so if the option is provided, it must come with an argument",
                                                     OPTION_REQUIRED_ARGUMENT_NOT_PROVIDED);
                                }
                                else {
                                    try {
                                        return conversion_t()(*subcommand.inputs.options_to_values.at(*short_name)); // no need for has_value check here; returning an empty optional is valid
                                    }
                                    catch(user_error& e) {
                                        throw user_error("Error initializing " + main_command_or_subcommand + " \"" + to_string(subcommand.name) + "\" option \"" + *short_name + "\" (full name \"" + canonical_name + "\"). Details: " + e.what(), e.error_type());
                                    }
                                }
                            }
                            else {
                                return {};
                            }
                        }
                    }
                    else {
                        if(!subcommand.inputs.options_to_values.contains(canonical_name) &&
                           (short_name && !subcommand.inputs.options_to_values.contains(*short_name))) {

                            throw user_error(main_command_or_subcommand + " \""  + to_string(subcommand.name) + "\" was not provided with required option \"" + canonical_name + '"', REQUIRED_OPTION_NOT_PROVIDED);
                        }
                        else if((subcommand.inputs.options_to_values.contains(canonical_name) && !subcommand.inputs.options_to_values.at(canonical_name).has_value()) ||
                                (short_name && subcommand.inputs.options_to_values.contains(*short_name) && !subcommand.inputs.options_to_values.at(*short_name).has_value())) {

                            throw user_error(main_command_or_subcommand + " \""  + to_string(subcommand.name) + "\" option \""
                                             + canonical_name + "\" requires an argument, but one was not provided "
                                                                "(expected an argument of type "
                                             + conversion_t::type_string.make_lowercase_and_convert_underscores().string() + ')',
                                             OPTION_REQUIRED_ARGUMENT_NOT_PROVIDED);
                        }
                        else { // by this point, none of the optionals we're interested in are empty
                            if(subcommand.inputs.options_to_values.contains(canonical_name)) {
                                try {
                                    return conversion_t()(*subcommand.inputs.options_to_values.at(canonical_name));
                                }
                                catch(user_error& e) {
                                    throw user_error("Error initializing " + main_command_or_subcommand + " \""
                                                     + to_string(subcommand.name) + "\" option \"" + canonical_name + "\". Details: " + e.what(), e.error_type());
                                }
                            }
                            else { // has_short_name is guaranteed to be true at this point
                                try {
                                    return conversion_t()(*subcommand.inputs.options_to_values.at(*short_name));
                                }
                                catch(user_error& e) {
                                    throw user_error("Error initializing " + main_command_or_subcommand + " \"" + to_string(subcommand.name) + "\" option \"" + *short_name + "\" (full name \"" + canonical_name + "\"). Details: " + e.what(), e.error_type());
                                }
                            }
                        }
                    }
                }
                else if constexpr(argument_info_t<last>::is_positional) { // positional
                    --cumulative_positional_index; // because we've already skipped over the actual parameter for the positional, cumulative_positional_index will be too big

                    if constexpr(last::optional) {
                        if(cumulative_positional_index < subcommand.inputs.positional_args.size()) {
                            try {
                                return conversion_t()(subcommand.inputs.positional_args[cumulative_positional_index]);
                            }
                            catch(user_error& e) {
                                throw user_error("Error initializing " + main_command_or_subcommand + " \"" + to_string(subcommand.name) + "\" positional argument \"" + canonical_name + "\". Details: " + e.what(), e.error_type());
                            }
                        }
                        else {
                            return {}; // empty optional
                        }
                    }
                    else {
                        if((cumulative_positional_index >= subcommand.inputs.positional_args.size())) {
                            throw user_error(main_command_or_subcommand + " \""  + to_string(subcommand.name) + "\" required positional argument \"" + canonical_name + "\" was not provided (expected an argument of type " + conversion_t::type_string.string() + ')', REQUIRED_POSITIONAL_NOT_PROVIDED);
                        }
                        else {
                            try {
                                return conversion_t()(subcommand.inputs.positional_args[cumulative_positional_index]);
                            }
                            catch(user_error& e) {
                                throw user_error("Error initializing " + main_command_or_subcommand + " \"" + to_string(subcommand.name) + "\" positional argument \"" + canonical_name + "\". Details: " + e.what(), e.error_type());
                            }
                        }
                    }
                }
                else { // variadic
                    T ret; // T is a vector
                    for(unsigned i = cumulative_positional_index; i < subcommand.inputs.positional_args.size(); ++i) {
                        try {
                            ret.emplace_back(conversion_t()(subcommand.inputs.positional_args[i]));
                        }
                        catch(user_error& e) {
                            throw user_error("Error initializing " + main_command_or_subcommand + " \"" + to_string(subcommand.name) + "\" variadic argument pack \"" + canonical_name + "\" at argument index " + std::to_string(i) + ". Details: " + e.what(), e.error_type());
                        }
                    }
                    return ret;
                }
            }
        }
        else {
            //using conversion_t = typename T::conversion_t;

            std::string canonical_name = T::name.string();

            std::optional<std::string> short_name;

            if constexpr(T::has_short_name) {
                short_name = std::string{T::short_name};
            }

            if constexpr(arg_info_t::is_option) {
                if constexpr(T::optional && T::argument_optional) {
                    if(subcommand.inputs.options_to_values.contains(canonical_name)) {
                        try {
                            return {subcommand.inputs.options_to_values.at(canonical_name)}; // no need for has_value check here; returning an empty optional is valid
                        }
                        catch(user_error& e) {
                            throw user_error("Error initializing " + main_command_or_subcommand + " \"" + to_string(subcommand.name) + "\" option \"" + canonical_name + "\". Details: " + e.what(), e.error_type());
                        }
                    }
                    else if(short_name && subcommand.inputs.options_to_values.contains(*short_name)) {
                        try {
                            return {subcommand.inputs.options_to_values.at(*short_name)}; // no need for has_value check here; returning an empty optional is valid
                        }
                        catch(user_error& e) {
                            throw user_error("Error initializing " + main_command_or_subcommand + " \"" + to_string(subcommand.name) + "\" option \"" + *short_name + "\" (full name \"" + canonical_name + "\"). Details: " + e.what(), e.error_type());
                        }
                    }
                    else {
                        return {}; // default constructor for optional option with optional argument inits to empty optional and was_included to false
                    }
                }
                else {
                    return {}; // this variable is a dummy
                }
            }
            else {
                return {}; // dummy
            }

        }
    }

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

    template<typename T, typename U, typename...Ts>
    constexpr bool pack_contains_short_name_func() {
        if constexpr(argument_info_t<T>::has_short_name && argument_info_t<U>::has_short_name) {
            if constexpr(sizeof...(Ts) > 0) {
                return (T::short_name == U::short_name) || pack_contains_short_name_func<T, Ts...>();
            }
            else {
                return (T::short_name == U::short_name);
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
        if constexpr(argument_info_t<T>::has_long_name && argument_info_t<U>::has_long_name) {
            if constexpr(sizeof...(Ts) > 0) {
                return (T::name == U::name) || pack_contains_short_name_func<T, Ts...>();
            }
            else {
                return (T::name == U::name);
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

    template<typename return_t, typename arg_t, typename...arg_ts>                      // don't actually care about func, just need to deduce args
    void generate_input_info_and_docs(subcommand_inputs_info_t& info, subcommand_documentation_t& documentation, return_t(*func)(arg_t, arg_ts...) = nullptr) {
        using T = std::remove_cvref_t<arg_t>;
        using arg_info_t = argument_info_t<T>;

        if constexpr(!arg_info_t::is_raw_type) {
            using type = std::remove_cvref_t<arg_t>;

            static_assert(no_repeated_short_names_v<arg_t, arg_ts...>, "multiple flags/options cannot share a short name");
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
            generate_input_info_and_docs<return_t, arg_ts...>(info, documentation);
        }
    }

    inline void generate_input_info_and_docs(subcommand_inputs_info_t& info, subcommand_documentation_t& documentation, void(*func)() = nullptr) {
        // do nothing
    }



    void default_help_callback(const command_context_t& cppli_current_command,

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

                               const option<unsigned, conversions::conversion_t<unsigned>, false, "recursion", "how many levels of nested subcommands to print. 0 prints none", "unsigned integer", true, false, 'r'>&, const std::optional<unsigned>& recursion);

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
            subcommand_name_t temp = name;
            temp.push_back("help");

            if(!subcommand_name_to_func().contains(temp)) {
                register_command<default_help_callback>(temp, "print help for this command", true);
            }
        }

        return {};
    }
}
}