#ifndef _LTD_INCLUDE_FLAGS_HPP_
#define _LTD_INCLUDE_FLAGS_HPP_

#include <string>
#include <vector>

#include "stddef.hpp"
#include "err.hpp"

namespace ltd
{

    /**
     * @brief
     * Provides functionalities for parsing command line arguments.
     * 
     * @details
     * 
     */
    class Flags
    {
    private:
        class ShortFlag
        {
        private:
            char flag;
            int  value;
            String description;

        public:
            ShortFlag();
            ShortFlag(const ShortFlag& other);
            ShortFlag(char flag, int value, const String& description);

            char get_flag() const;
            int get_val() const;
            String get_description() const;

            void inc_val();
        };

        class LongFlag
        {
        private:
            String flag;
            String value;
            String description;

        public:
            LongFlag();
            LongFlag(const LongFlag& other);
            LongFlag(const String& flag, const String& value, const String& description);

            String get_flag() const;
            String get_val() const;
            String get_description() const;

            void set_val(const String& val);

        };

        class Command
        {
        private:
            String command;
            String description;
            int    value;

        public:
            Command();
            Command(const Command& other);
            Command(const String& cmd, int value, const String& desc);

            String get_command() const;
            String get_description() const;
            int  get_val() const;
        };
        
        using ShortFlags = std::vector<ShortFlag>;
        using LongFlags = std::vector<LongFlag>;
        using Commands = std::vector<Command>;

    private:

        StringList args;         
        ShortFlags shorts;
        LongFlags  longs;

        String err_param;

        int      command = -1;
        Commands commands;

    public:
        Flags(int argc, char *argv[]);

        void add_flag(char flag, int defval, const String &desc);
        void add_flag(const String& flag, const String& defval, const String& desc);
        void add_command(const String& cmd, int defval, const String& description);

        multi_ret<int,err>    get_int(char c) const;
        multi_ret<String,err> get_text(const String& param) const;
        
        bool is_exist(char c) const;
        bool is_exist(const String& param) const;

        int get_cmd() const;

        String get_failed_param() const;

        err parse();

        String at(int index) const;

        void print_help();

    private:

        int get_flag_id(char c) const;
        int get_flag_id(const String &param) const;

        err parse_long_flag(const String& arg);
        err parse_short_flag(const String& arg);

        int map_command(const String& cmd) const;

    };  // class Flags
} // namespace

#endif // _LTD_INCLUDE_FLAGS_HPP_