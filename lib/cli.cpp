#include <stdexcept>

#include "../inc/ltd/cli.hpp"
#include "../inc/ltd/fmt.hpp"

namespace ltd
{
    cli::flag_arg::flag_arg() : flag(0), counter(nullptr)
    {
        
    }

    cli::flag_arg::flag_arg(const flag_arg& other)
    {
        flag = other.flag;
        counter = other.counter;
        description = other.description;
    }

    cli::flag_arg::flag_arg(char flag, int *read_counter, const string& description)
    {
        this->counter = read_counter;
        this->flag = flag;
        this->description = description;
    }

    char cli::flag_arg::get_flag() const
    {
        return flag;
    }

    string cli::flag_arg::get_description() const
    {
        return description;
    }

    bool cli::flag_arg::read_flag(char flag)
    {
        if (this->flag == flag && counter != nullptr) {
            *counter = *counter + 1;
            return true;
        }

        return false;
    }

    cli::param_arg::param_arg()
    {

    }

    cli::param_arg::param_arg(const param_arg& other)
    {
        flag = other.flag;
        value = other.value;
        description = other.description;
    }

    cli::param_arg::param_arg(const string& flag, int *value, const string& description)
    {
        this->flag = flag;
        this->value = value;
        this->description = description;
    }

    cli::param_arg::param_arg(const string& flag, float *value, const string& description)
    {
        this->flag = flag;
        this->value = value;
        this->description = description;
    }

    cli::param_arg::param_arg(const string& flag, string *value, const string& description)
    {
        this->flag = flag;
        this->value = value;
        this->description = description;
    }

    cli::param_arg::param_arg(const string& flag, string_list *values, const string& description)
    {
        this->flag = flag;
        this->value = values;
        this->description = description;
    }

    string cli::param_arg::get_flag() const
    {
        return flag;
    }

    string cli::param_arg::get_description() const
    {
        return description;
    }

    bool cli::param_arg::read_flag(const string& argument)
    {
        auto tokens = split(argument, "=");

        if (tokens.size() != 2)
            return false;

        if (this->flag == tokens[0]) {
            auto param = tokens[1];

            if (is_string()) {
                string *val = std::get<string*>(value);
                *val = param;
            } else if (is_int()) {
                int *val = std::get<int*>(value);
                *val = std::stoi(param);
            } else if (is_float()) {
                float *val = std::get<float*>(value);
                *val = std::stof(param);
            } else if (is_string_list()) {
                string_list *values = std::get<string_list*>(value);
                auto arg_params = split(param, ":");
                
                for (auto arg_param : arg_params) {
                    values->push_back(arg_param);
                }
            } else
                return false;

            return true;
        }

        return false;
    }

    bool cli::param_arg::is_string() const
    {
        return std::holds_alternative<string*>(value);
    }

    bool cli::param_arg::is_string_list() const
    {
        return std::holds_alternative<string_list*>(value);
    }

    bool cli::param_arg::is_int() const
    {
        return std::holds_alternative<int*>(value);
    }

    bool cli::param_arg::is_float() const
    {
        return std::holds_alternative<float*>(value);
    }

    cli::command::command()
    {
        value = -1;
    }

    cli::command::command(const command& other)
    {
        name = other.name;
        value = other.value;
        description = other.description;
    }

    cli::command::command(const string& name, int value, const string& description)
    {
        this->name = name;
        this->value = value;
        this->description = description;
    }

    string cli::command::get_name() const
    {
        return name;
    }

    string cli::command::get_description() const
    {
        return description;
    }

    int cli::command::get_value() const
    {
        return value;
    }

    cli::cli(int argc, char *argv[]) 
    {
        for(int i=1; i<argc; i++) {
            args.push_back(argv[i]);
        }
    }

    void cli::bind_flag(int& out_val, char flag, const string &description)
    {
        flags.emplace_back(flag, &out_val, description);
    }

    void cli::bind_param(string& out_val, const string& param, const string& description)
    {
        params.emplace_back(param, &out_val, description);
    }

    void cli::bind_param(string_list& out_val, const string& param, const string& description)
    {
        params.emplace_back(param, &out_val, description);
    }

    void cli::bind_param(int& out_val, const string& param, const string& description)
    {
        params.emplace_back(param, &out_val, description);
    }

    void cli::bind_param(float& out_val, const string& param, const string& description)
    {
        params.emplace_back(param, &out_val, description);
    }

    void cli::add_command(const string& name, int defval, const string& description)
    {
        commands.emplace_back(name, defval, description);
    }

    err cli::parse()
    {
        bool first = true;
        for(const string& arg : args) {
            if (first) {
                first = false;
                if (arg.at(0) != '-') {
                    cmd_value = map_command(arg);
                    continue;
                }
            }

            if (arg.at(0) == '-' && arg.length() > 1) {
                if (arg.at(1) == '-' && arg.length() > 2) {
                    parse_param(arg);
                } else {
                    parse_flag(arg);
                }
            } else
                return err::invalid_argument;
        } // for

        return err::no_error;
    }

    err cli::parse_param(const string& arg)
    {
        if (arg.substr(0, 2) != "--") 
            return err::invalid_argument;

        auto keyval = arg.substr(2);
        
        for (param_arg &param : params) {
            try {
                if (param.read_flag(keyval) == true)
                    return err::no_error;
            } catch (std::invalid_argument const& ex)
            {
                // TODO:: log error
                return err::invalid_argument;
            } catch (std::out_of_range const& ex)
            {
                // TODO:: log error
                return err::out_of_range;
            }
        }
        return err::not_found;
    }

    err cli::parse_flag(const string& arg)
    {
        if(arg.at(0) != '-' || arg.size() <= 1)
            return err::invalid_argument;
        
        auto opts = arg.substr(1);

        std::size_t counter = 0;
        for (auto c : opts) {
            bool matched = false;
            for (flag_arg &opt : flags) {
                if(opt.read_flag(c) == true) {
                    matched = true;
                    counter++;
                    break;
                }
            }
            if (matched == false)
                return err::not_found;
        }

        return err::no_error;
    }

    int cli::map_command(const string& name) const
    {
        for (const command& cmd : commands) {
            if(cmd.get_name() == name) {
                return cmd.get_value();
            }
        }

        return -1;
    }

    int cli::get_command() const
    {
        return cmd_value;
    }

    void cli::print_help() 
    {
        fmt::println("Commands and flags:");
        for(auto cmd : commands) {
            fmt::println("  %-14s %s", cmd.get_name(), cmd.get_description());
        }

        fmt::println("");

        for (auto flag : flags) {
            fmt::println("  -%-13c %s", flag.get_flag(), flag.get_description());
        }

        fmt::println("");

        for (auto param : params) {
            fmt::println("  --%-12s %s", param.get_flag(), param.get_description());
        }
    }

    multi_ret<string,err> cli::at(int index) const
    {
        if (index >= 0 && index < args.size()) {
            return {args.at(index), err::no_error};
        }

        return {"", err::out_of_range};
    }
}