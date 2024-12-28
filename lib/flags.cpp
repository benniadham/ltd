#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

#include "../inc/flags.hpp"
#include "../inc/fmt.hpp"

namespace ltd
{
    Flags::ShortFlag::ShortFlag() : flag(0), value(0)
    {
        
    }

    Flags::ShortFlag::ShortFlag(const ShortFlag& other)
    {
        flag = other.flag;
        value = other.value;
        description = other.description;
    }

    Flags::ShortFlag::ShortFlag(char flag, int value, const String& description)
    {
        this->flag = flag;
        this->value = value;
        this->description = description;
    }

    char Flags::ShortFlag::get_flag() const
    {
        return flag;
    }

    int Flags::ShortFlag::get_val() const
    {
        return value;
    }

    String Flags::ShortFlag::get_description() const
    {
        return description;
    }

    void Flags::ShortFlag::inc_val()
    {
        value++;
    }

    Flags::LongFlag::LongFlag() 
    {

    }

    Flags::LongFlag::LongFlag(const LongFlag& other)
    {
        flag = other.flag;
        value = other.value;
        description = other.description;
    }

    Flags::LongFlag::LongFlag(const String& flag, const String& value, const String& description)
    {
        this->flag = flag;
        this->value = value;
        this->description = description;
    }

    String Flags::LongFlag::get_flag() const
    {
        return flag;
    }

    String Flags::LongFlag::get_val() const
    {
        return value;
    }

    String Flags::LongFlag::get_description() const
    {
        return description;
    }

    void Flags::LongFlag::set_val(const String& val)
    {
        this->value = val;
    }

    Flags::Command::Command()
    {
        value = -1;
    }

    Flags::Command::Command(const Command& other)
    {
        command = other.command;
        value = other.value;
        description = other.description;
    }

    Flags::Command::Command(const String& cmd, int val, const String& desc)
    {
        command = cmd;
        value = val;
        description = desc;
    }

    String Flags::Command::get_command() const
    {
        return command;
    }

    String Flags::Command::get_description() const
    {
        return description;
    }

    int  Flags::Command::get_val() const
    {
        return value;
    }

    Flags::Flags(int argc, char *argv[]) 
    {
        for(int i=1; i<argc; i++) {
            args.push_back(argv[i]);
        }
    }

    void Flags::add_flag(char flag, int defval, const String &desc)
    {
        ShortFlag short_flag = ShortFlag(flag, defval, desc);
        shorts.push_back(short_flag);
    }

    void Flags::add_flag(const String &flag, const String &defval, const String &desc)
    {
        LongFlag long_flag = LongFlag(flag, defval, desc);
        longs.push_back(long_flag);
    }
    void Flags::add_command(const String& cmd, int defval, const String& description)
    {
        Command command = Command(cmd, defval, description);
        commands.push_back(command);
    }

    err Flags::parse_long_flag(const String& arg)
    {
        if (arg.substr(0, 2) != "--") 
            return err::invalid_argument;

        auto keyval = arg.substr(2);
        auto pos = keyval.find("=");

        if(pos != String::npos ) 
        {
            auto key = keyval.substr(0, pos);
            auto val = keyval.substr(pos+1);

            auto id = get_flag_id(key);
            if (id == -1) {
                err_param = key;
                return err::not_found;
            }

            longs[id].set_val(val);
        } 
        else 
        {
            auto id = get_flag_id(keyval);

            if (id == -1) {
                err_param = keyval;
                return err::not_found;
            }

            longs[id].set_val("");
        }

        return err::no_error;
    }

    err Flags::parse_short_flag(const String& arg)
    {
        if(arg.at(0) != '-' || arg.size() <= 1)
            return err::invalid_argument;
        
        auto flags = arg.substr(1);

        for (auto c : flags) {
            auto id = get_flag_id(c);
            if (id == -1) {
                err_param = c;
                return err::not_found;
            }

            shorts[id].inc_val();
        }

        return err::no_error;
    }

    err Flags::parse()
    {
        bool first = true;
        for(const String& arg : args) {

            if (first) {
                first = false;

                if (arg.at(0) != '-') {
                    try {
                        command = map_command(arg);
                    } catch(const std::out_of_range& ex) {
                        command = -1;
                    }
                    continue;
                }
            }

            auto e = parse_long_flag(arg);

            if (e == err::invalid_argument) {
                e = parse_short_flag(arg);
                if (e == err::invalid_argument) {
                    continue;
                }
            }

            if (e == err::not_found) 
                return e;
        }

        return err::no_error;
    }

    int Flags::get_flag_id(char c) const
    {
        int counter = -1;
        for (const ShortFlag &flag : shorts) {
            counter++;
            if (flag.get_flag() == c) {
                return counter;
            }
        }

        return -1;
    }

    int Flags::get_flag_id(const String &flag) const
    {
        int counter = -1;
        for (const LongFlag &long_flag : longs) {
            counter++;
            if (long_flag.get_flag() == flag) {
                return counter;
            }
        }

        return -1;
    }

    multi_ret<int,err> Flags::get_int(char c) const
    {
        int id = get_flag_id(c);
        if(id >= 0 && id < shorts.size())
            return {shorts[id].get_val(), err::no_error};

        return {-1, err::not_found};
    }

    multi_ret<String, err> Flags::get_text(const String &param) const
    {
        int id = get_flag_id(param);

        if(id >= 0 && id < longs.size())
            return {longs[id].get_val(), err:: no_error};

        return {"", err::not_found};
    }

    bool Flags::is_exist(char c) const
    {
        auto [v, e] = get_int(c);
        return v>0;
    }

    bool Flags::is_exist(const String& param) const
    {
        auto [v, e] = get_text(param);
        return e != err::not_found;
    }

    int Flags::get_cmd() const
    {
        return command;
    }

    String Flags::get_failed_param() const
    {
        return err_param;
    }

    int Flags::map_command(const String& cmd) const
    {
        for (const Command& command : commands) {
            if(command.get_command() == cmd) {
                return command.get_val();
            }
        }

        return -1;
    }

    String Flags::at(int index) const
    {
        if (index >= 0 && index < args.size()) {
            return args.at(index);
        }

        return "";
    }

    void Flags::print_help(int level) 
    {
        for(auto cmd : commands) {
            fmt::println("  %-10s %s", cmd.get_command(), cmd.get_description());
        }

        fmt::println("");

        for (auto short_opt : shorts) {
            fmt::println("  -%-9c %s", short_opt.get_flag(), short_opt.get_description());
        }

        fmt::println("");

        for (auto long_opt : longs) {
            fmt::println("  --%-8s %s", long_opt.get_flag(), long_opt.get_description());
        }
    }
} // namespace