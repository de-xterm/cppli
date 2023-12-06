
#include <tuple>
#include <sstream>

#include "subcommand.h"
#include "documentation.h"
#include "configuration.h"

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
                   << " flag --" << short_flag_or_option << " included multiple times";

                if(subcommand.inputs.flags.contains(long_name)) {
                    ss << "(previously included with long name \"" << long_name << '\"';
                }
                print_throw_or_do_nothing(FLAG_INCLUDED_MULTIPLE_TIMES, ss.str());
            }

        }
        else {
            if(subcommand.inputs.options_to_values.contains(short_flag_or_option) ||
               subcommand.inputs.options_to_values.contains(long_name)) {

                ss << (subcommand.name == command_name_t{"MAIN"} ? "main command" : "subcommand") << ' ' << to_string(subcommand.name)
                   << " option --" << short_flag_or_option << " included multiple times";

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