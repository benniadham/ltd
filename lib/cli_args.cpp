#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

#include "../inc/ltd/cli_args.hpp"
#include "../inc/ltd/fmt.hpp"

namespace ltd
{
    cli_args::flag::flag() : value(0), counter(0)
    {
        
    }

    cli_args::flag::flag(const flag& other)
    {
        value = other.value;
        counter = other.counter;
        description = other.description;
    }

    cli_args::flag::flag(char value, int counter, const string& description)
    {
        this->value = value;
        this->counter = counter;
        this->description = description;
    }

    char cli_args::flag::get_flag() const
    {
        return value;
    }

    int cli_args::flag::get_counter() const
    {
        return counter;
    }

    string cli_args::flag::get_description() const
    {
        return description;
    }

    void cli_args::flag::inc_counter()
    {
        counter++;
    }

    cli_args::parameter::parameter() 
    {

    }

    cli_args::parameter::parameter(const parameter& other)
    {
        name    = other.name;
        value   = other.value;
        description = other.description;
    }

    cli_args::parameter::parameter(const string& name, const string& value, const string& description)
    {
        this->name = name;
        this->value = value;
        this->description = description;
    }

    string cli_args::parameter::get_name() const
    {
        return name;
    }

    string cli_args::parameter::get_value() const
    {
        return value;
    }

    string cli_args::parameter::get_description() const
    {
        return description;
    }

    void cli_args::parameter::set_value(const string& value)
    {
        this->value = value;
    }

    cli_args::command::command()
    {
        value = -1;
    }

    cli_args::command::command(const command& other)
    {
        name = other.name;
        value = other.value;
        description = other.description;
    }

    cli_args::command::command(const string& name, int value, const string& description)
    {
        this->name = name;
        this->value = value;
        this->description = description;
    }

    string cli_args::command::get_name() const
    {
        return name;
    }

    string cli_args::command::get_description() const
    {
        return description;
    }

    int  cli_args::command::get_value() const
    {
        return value;
    }

    cli_args::cli_args(int argc, char *argv[]) 
    {
        for(int i=1; i<argc; i++) {
            args.push_back(argv[i]);
        }
    }

    void cli_args::add_flag(char value, int defcount, const string &description)
    {
        flag short_flag = flag(value, defcount, description);
        flags.push_back(short_flag);
    }

    void cli_args::add_param(const string &name, const string &defval, const string &description)
    {
        parameter param = parameter(name, defval, description);
        params.push_back(param);
        fmt::println("add_param(%s)", param.get_name());
    }

    void cli_args::add_command(const string& name, int defval, const string& description)
    {
        command cmd = command(name, defval, description);
        commands.push_back(cmd);
    }

    err cli_args::parse_param(const string& arg)
    {
        if (arg.substr(0, 2) != "--") 
            return err::invalid_argument;

        auto keyval = arg.substr(2);
        auto pos = keyval.find("=");

        if(pos != string::npos ) 
        {
            auto key = keyval.substr(0, pos);
            auto val = keyval.substr(pos+1);

            auto id = get_param_id(key);
            if (id == -1) {
                err_param = key;
                return err::not_found;
            }

            params[id].set_value(val);
        } 
        else 
        {
            auto id = get_param_id(keyval);

            if (id == -1) {
                err_param = keyval;
                return err::not_found;
            }

            params[id].set_value("");
        }

        return err::no_error;
    }

    err cli_args::parse_flag(const string& arg)
    {
        if(arg.at(0) != '-' || arg.size() <= 1)
            return err::invalid_argument;
        
        auto opts = arg.substr(1);

        for (auto c : opts) {
            auto id = get_flag_id(c);
            if (id == -1) {
                err_param = c;
                return err::not_found;
            }

            flags[id].inc_counter();
        }

        return err::no_error;
    }

    err cli_args::parse()
    {
        bool first = true;
        for(const string& arg : args) {

            if (first) {
                first = false;

                if (arg.at(0) != '-') {
                    try {
                        cmd_value = map_command(arg);
                    } catch(const std::out_of_range& ex) {
                        cmd_value = -1;
                    }
                    continue;
                }
            }

            auto e = parse_param(arg);

            if (e == err::invalid_argument) {
                e = parse_flag(arg);
                if (e == err::invalid_argument) {
                    continue;
                }
            }

            if (e == err::not_found) 
                return e;
        }

        return err::no_error;
    }

    int cli_args::get_flag_id(char c) const
    {
        int counter = -1;
        for (const flag &opt : flags) {
            counter++;
            if (opt.get_flag() == c) {
                return counter;
            }
        }

        return -1;
    }

    int cli_args::get_param_id(const string &name) const
    {
        int counter = -1;
        for (const parameter &param : params) {
            counter++;
            if (param.get_name() == name) {
                return counter;
            }
        }

        return -1;
    }

    multi_ret<int,err> cli_args::get_counter(char opt) const
    {
        int id = get_flag_id(opt);
        if(id >= 0 && id < flags.size())
            return {flags[id].get_counter(), err::no_error};

        return {-1, err::not_found};
    }

    multi_ret<string, err> cli_args::get_value(const string &param_name) const
    {
        int id = get_param_id(param_name);

        if(id >= 0 && id < params.size())
            return {params[id].get_value(), err:: no_error};

        return {"", err::not_found};
    }

    bool cli_args::is_flag_exist(char opt) const
    {
        auto [v, e] = get_counter(opt);
        return v>0;
    }

    bool cli_args::is_param_exist(const string& name) const
    {
        auto [v, e] = get_value(name);
        return e != err::not_found;
    }

    int cli_args::get_command() const
    {
        return cmd_value;
    }

    string cli_args::get_failed_param() const
    {
        return err_param;
    }
 
    int cli_args::map_command(const string& name) const
    {
        for (const command& cmd : commands) {
            if(cmd.get_name() == name) {
                return cmd.get_value();
            }
        }

        return -1;
    }

    multi_ret<string,err> cli_args::at(int index) const
    {
        if (index >= 0 && index < args.size()) {
            return {args.at(index), err::no_error};
        }

        return {"", err::index_out_of_bound};
    }

    void cli_args::print_help() 
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
            fmt::println("  --%-12s %s", param.get_name(), param.get_description());
        }
    }
} // namespace