#ifndef _LTD_INCLUDE_CLI_HPP_
#define _LTD_INCLUDE_CLI_HPP_

#include <string>
#include <vector>
#include <variant>

#include "stddef.hpp"
#include "err.hpp"

namespace ltd
{
    class cli
    {
    private:
        class flag_arg
        {
        private:
            char flag    = 0;
            int *counter = nullptr;
            string description;

        public:
            // ctors
            flag_arg();
            flag_arg(const flag_arg& other);
            flag_arg(char flag, int *read_counter, const string& description);

            // Get the flag char value.
            char get_flag() const;

            // Get the description of this flag.
            string get_description() const;

            bool read_flag(char argument);
        };

        class param_arg
        {
        private:
            using param_value = std::variant<string*,int*,float*,string_list*>;

            string flag;
            string description;

            param_value value;

        public:
            // ctors
            param_arg();
            param_arg(const param_arg& other);
            param_arg(const string& flag, int *value, const string& description);
            param_arg(const string& flag, float *value, const string& description);
            param_arg(const string& flag, string *value, const string& description);
            param_arg(const string& flag, string_list *values, const string& description);

            /**
             * @brief
             * Get the name of the parameter.
             * 
             * @return string The name of the parameter
             */
            string get_flag() const;

            /**
             * @brief
             * Get the string description of the parameter.
             * 
             * @return string The description of the parameter.
             */
            string get_description() const;

            bool is_string() const;
            bool is_string_list() const;
            bool is_int() const;
            bool is_float() const;

            bool read_flag(const string& argument);
        };

        /**
         * @brief
         * Implements commands for command line arguments.
         * 
         * @details
         * Class 'command' provides implementation of command for command line
         * arguments. It is a string label that comes right after the executable
         * name. I.e. the argv[1].
         */
        class command
        {
        private:
            string name;                // The command name.
            string description;         // Text description for help.
            int    value;               // Integer representation value for the command.

        public:
            // ctors
            command();
            command(const command& other);
            command(const string& name, int value, const string& description);

            /**
             * @brief
             * Get the name of the command
             * 
             * @return string The name of the command
             */
            string get_name() const;

            /**
             * @brief
             * Get the text description of the command.
             * 
             * @return string Text description of the command.
             */
            string get_description() const;

            /**
             * @brief
             * Get integer value of the command.
             * 
             * @return int The integer value of the command.
             */
            int    get_value() const;
        };

    private:
        using flag_list = std::vector<flag_arg>;
        using param_list = std::vector<param_arg>;
        using command_list = std::vector<command>;

        string_list args;         
        flag_list   flags;
        param_list  params;

        string err_param;

        int          cmd_value = -1;
        command_list commands;

    public:
        cli(int argc, char *argv[]);

        /**
         * @brief
         * Bind an integer variable to a flag in the argument list.
         *
         * @param out_val     The variable to receive the flag value.
         * @param flag        The character repesenting the option switch.
         * @param description The descriptive text for help.    
         */
        void bind_flag(int& out_val, char flag, const string &description);
        
        /**
         * @brief
         * Bind a string variable to a param in the argument list.
         * 
         * @param out_val     The variable to receive the param value.
         * @param param       The display name of the parameter.
         * @param description The description text for help.
         */
        void bind_param(string& out_val, const string& param, const string& description);
        
        /**
         * @brief
         * Bind a string list variable to list param of parameters in the argument
         * list separated by colon ':'. I.e.:
         *   --imports=ltd:motyf
         * 
         * @param out_val     The variable to receive the param value.
         * @param param       The display name of the parameter.
         * @param description The description text for help.
         */
        void bind_param(string_list& out_val, const string& param, const string& description);

        /**
         * @brief
         * Bind an integer variable to a param in the argument list.
         * 
         * @param out_val     The variable to receive the param value.
         * @param param       The display name of the parameter.
         * @param description The description text for help.
         */
        void bind_param(int& out_val, const string& param, const string& description);

        /**
         * @brief
         * Bind a float variable to a param in the argument list.
         * 
         * @param out_val     The variable to receive the param value.
         * @param param       The display name of the parameter.
         * @param description The description text for help.
         */
        void bind_param(float& out_val, const string& param, const string& description);

        /**
         * @brief
         * Add command to the argument list.
         * 
         * @param name        The display name of the command.
         * @param defval      The default value for command.
         * @param description The description text for help.
         */
        void add_command(const string& name, int defval, const string& description);

        /**
         * @brief
         * Get the translated integer value of the string command specified.
         * 
         * @return int The integer value of the specified command.
         */
        int get_command() const;
        
        /**
         * @brief
         * Parse the argc and argv from `main()`.
         * 
         * @return err The status of the function execution.
         */
        err parse();

        /**
         * @brief
         * Parse the parameter from the string argument and fill the bounded
         * variable.
         * 
         * @param arg The string argument.
         * @return err The error status fromt the function execution. 
         */
        err parse_param(const string& arg);

        /**
         * @brief
         * Parse one or more flag(s) from the string argument and fill the bounded
         * variable.
         * 
         * @param arg The string argument.
         * @return err The error status fromt the function execution. 
         */
        err parse_flag(const string& arg);

        /**
         * @brief
         * Prints all flags, params and commands for the use of printing help or usage.
         */
        void print_help();

        /**
         * @brief
         * Get an argument as string given the index.
         * 
         * @param index The index of the argument.
         * @return string The argument.
         * @return err The error status fromt the function execution.
         */
        multi_ret<string,err> at(int index) const;         
        
    private:
        /**
         * @brief
         * Get the mapped int value for the given command name.
         * 
         * @param name The command name.
         * @return int The integer value associated with the command.
         */
        int map_command(const string& name) const;
    };
} // namespace ltd
#endif // _LTD_INCLUDE_CLI_HPP_