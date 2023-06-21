
#include "subcommand.h"

#include <unordered_set>
#include <tuple>

namespace cppli::detail {

    // construct on first use idiom
    // more info here: https://isocpp.org/wiki/faq/ctors#construct-on-first-use-v2
    std::unordered_map<subcommand_name_t, subcommand_inputs_info_t, subcommand_name_hash_t>& subcommand_name_to_inputs_info() {
        static std::unordered_map<subcommand_name_t, subcommand_inputs_info_t, subcommand_name_hash_t> subcommand_name_to_inputs_info_;

        return subcommand_name_to_inputs_info_;
    }

    std::unordered_map<subcommand_name_t, subcommand_func_t, subcommand_name_hash_t>& subcommand_name_to_func() {
        static std::unordered_map<subcommand_name_t, subcommand_func_t, subcommand_name_hash_t> subcommand_name_to_func_;

        return subcommand_name_to_func_;
    }

    std::unordered_map<subcommand_name_t, subcommand_documentation_t, subcommand_name_hash_t>& subcommand_name_to_docs() {
        static std::unordered_map<subcommand_name_t, subcommand_documentation_t, subcommand_name_hash_t> subcommand_name_to_docs_;

        return subcommand_name_to_docs_;
    }

    bool flag_info_t::operator<(const flag_info_t& rhs) const {
        return (name < rhs.name);
    }

    flag_info_t::flag_info_t(const std::string& name, const std::string& documentation, char short_name) : name(name),
                                                                                                           documentation(
                                                                                                                   documentation),
                                                                                                           short_name(
                                                                                                                   short_name) {}

    bool option_info_t::operator<(const option_info_t& rhs) const {
        return std::tie(is_optional, argument_is_optional, name) <
               std::tie(rhs.is_optional, rhs.argument_is_optional, rhs.name);
    }

    option_info_t::option_info_t(const std::string& type, const std::string& name, const std::string& argument_text,
                                 const std::string& documentation, char short_name, bool is_optional,
                                 bool argument_is_optional) : type(type), name(name), argument_text(argument_text),
                                                              documentation(documentation), short_name(short_name),
                                                              is_optional(is_optional),
                                                              argument_is_optional(argument_is_optional) {}

    subcommand_documentation_t::subcommand_documentation_t(const std::string& name, const char* description) : name(name), description(description), is_namespace(false) {}

    bool subcommand_documentation_t::operator<(const subcommand_documentation_t& rhs) const {
        return (name.back() < rhs.name.back());
    }

    bool is_valid_subcommand(subcommand_name_t& parent_command_names, const std::string& arg) {
        auto temp = parent_command_names;
        temp.push_back(arg);
        if(subcommand_name_to_docs().contains(temp) || (temp.size() == 0)) {
            parent_command_names = std::move(temp);
            return true;
        }
        return false;
    }

    bool subcommand_takes_flag(const subcommand_name_t& subcommand, const std::string& flag_name) {
        return subcommand_name_to_inputs_info().contains(subcommand) &&
               subcommand_name_to_inputs_info().at(subcommand).flags.contains(flag_name);
    }

    bool subcommand_takes_option(const subcommand_name_t& subcommand, const std::string& option_name) {
        return subcommand_name_to_inputs_info().contains(subcommand) &&
               subcommand_name_to_inputs_info().at(subcommand).option_argument_is_optional.contains(option_name);
    }

    bool subcommand_option_argument_is_optional(const subcommand_name_t& subcommand, const std::string& option_name) {
        if(subcommand_name_to_inputs_info().at(subcommand).option_argument_is_optional.contains(option_name)) {
            return subcommand_name_to_inputs_info().at(subcommand).option_argument_is_optional.at(option_name);
        }
        return false;
    }

    bool is_namespace(const subcommand_name_t& subcommand) {
        return subcommand_name_to_docs().at(subcommand).is_namespace;
    }

    bool main_command_is_namespace() {
        return true; // TODO: actual implementation
    }

    static std::string program_name,
                       program_description;

    void set_program_name_and_description(std::string&& name, std::string&& description) {
        program_name        = std::move(name);
        program_description = std::move(description);
    }

    positional_info_t::positional_info_t(const std::string& type, const std::string& name,
                                         const std::string& documentation, bool optional) : type(type), name(name),
                                                                                               documentation(
                                                                                                       documentation),
                                                                                               optional(optional) {}

    std::size_t detail::subcommand_name_hash_t::operator()(const subcommand_name_t& name) const noexcept  {
        std::size_t hash = 0;
        for(const auto& e : name) {
            cppli::detail::hash_combine(hash, e);
        }

        return hash;
    }
    
    
}