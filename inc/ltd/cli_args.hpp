#ifndef _LTD_INCLUDE_CLI_ARGS_HPP_
#define _LTD_INCLUDE_CLI_ARGS_HPP_

#include <string>
#include <vector>

#include "stddef.hpp"
#include "err.hpp"

namespace ltd
{
    /**
     * @brief
     * Provides helper functions for handling with command-line arguments.
     * 
     * @details
     * Class cli_args implements command line arguments helper tools.
     * 
     * This class provides command line arguments parser and simple help system.
     * To use this class, initialize it with `argc` and `argv` taken from `main()`.
     * 
     * Bind local variable with an option by defining commands, flags ('-'),
     * parameters ('--') and description for help text.
     * 
     * Commands are a single word depicts the action to be taken, i.e:
     *  # ltd build 
     *  # ltd test
     * 
     * Flags are used for indicating switches, i.e. :
     *  - Using '-g' to indicates debug switch:
     *      `# ltd build -g`
     *  - Using -v' indicates verbosity switch. 3 switches mean 3 levels of verbosity:
     *      `# ltd build -g -vvv`
     * 
     * Parameterts are used to defines values, i.e. :
     *  - Using '--std' to provide C++ standard:
     *      `# ltd build -g -vvv --std=c++17`
     * 
     * commands are 
     * 
     * ```C++
     *          int main(int argc, char *argv[])
     *          {
     *              cli_args args(argc, argv);
     * 
     *              int debug     = 0;
     *              int verbosity = 0;
     * 
     *              args.add_param("verbosity", "1", "Specifies verbosity level 1, 2, 3 or 4");
     *              args.add_param("std", "", "Specifies cpp standards");
     * 
     *              args.add_flag('v', 0, "Sets verbosity level 1-4");
     *              args.add_flag('g', 0, "Debug mode");
     * 
     *              args.add_command("ls",  CMD_LS, "List all projects in the workspace");
     *              args.add_command("pwd", CMD_PWD, "Show currect active project");
     *              args.add_command("cd",  CMD_CD, "Change project directory");
     *       
     *              args.add_command("build", CMD_BUILD, "Build the current active project");
     *              args.add_command("clean", CMD_CLEAN, "Clean the current active project");
     *              args.add_command("test",  CMD_TEST, "Run tests");
     *              args.add_command("help",  CMD_HELP, "Show this help");
     * 
     *              args.parse();
     * 
     *              if (args.size() < 2)
     *              {
     *                  print_help();
     *                  args.print_help();
     *                  return 1;
     *              }
     * 
     *              // .....
     * 
     *              return 0;
     *          }
     * ```
     */
    class cli_args
    {
    private:
        /**
         * @brief
         * Implements flags or switchs for command line arguments.
         * 
         * @details
         * Class 'flag' provides the implementation for single dash switches 
         * or options normally found in command line arguments. i.e '-h', '-v' etc.
         * 
         * It supports multiple flags in a single dash i.e. `tar -xzv`.
         * 
         * To check the occurence of a flag,t check if the counter > 0.
         * 
         * A flag can occur multiple times, increasing its counter to more than 1.
         */
        class flag
        {
        private:
            char value;                      // The flag.
            int  counter;                   // The counter for many occurence.

            string description;             // A text description for help.

        public:
            // ctors
            flag();
            flag(const flag& other);
            flag(char value, int counter, const string& description);

            // Get the flag char value.
            char get_flag() const;

            // Get flag occurence.
            int  get_counter() const;
            
            // Get the description of this flag.
            string get_description() const;

            // Increment the occurence counter for this flag.
            void inc_counter();
        };

        /**
         * @brief
         * Impements parameters for command line arguments.
         * 
         * @details
         * Class `parameters` provides implemetation for passing parameters in
         * command line arguments. It uses double dash '--' symbol followed by
         * the name of the parameters, a '=' and then the value of the parameters.
         * 
         * For example: `--std=c++17`
         * 
         * There should not be a space in the parameter argument.
         */
        class parameter
        {
        private:
            string name;                    // The name of the parameter.
            string value;                   // The value of the parameter.
            string description;             // The text description for help.                         

        public:
            // ctors
            parameter();
            parameter(const parameter& other);
            parameter(const string& name, const string& value, const string& description);

            /**
             * @brief
             * Get the name of the parameter.
             * 
             * @return string The name of the parameter
             */
            string get_name() const;

            /**
             * @brief
             * Get the value of the parameter.
             * 
             * @return string The value of the parameter.
             */
            string get_value() const;

            /**
             * @brief
             * Get the string description of the parameter.
             * 
             * @return string The description of the parameter.
             */
            string get_description() const;

            /**
             * @brief 
             * Set the value of the parameter.
             * 
             * @param value The string value for the parameter.
             */
            void set_value(const string& value);
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
        using flag_list = std::vector<flag>;
        using param_list = std::vector<parameter>;
        using command_list = std::vector<command>;

        string_list args;         
        flag_list   flags;
        param_list  params;

        string err_param;

        int          cmd_value = -1;
        command_list commands;

    public:
        // ctors
        cli_args(int argc, char *argv[]);

        /**
         * @brief
         * Add a flag to the argument list.
         * 
         * @param value       The character repesenting the option switch.
         * @param defcount    The default/start value for the counter.
         * @param description The descriptive text for help.    
         */
        void add_flag(char value, int defcount, const string &description);
        
        /**
         * @brief
         * Add a parameter to the argument list.
         * 
         * @param name        The display name of the parameter.
         * @param defval      The default value for the parameter.
         * @param description The description text for help.
         */
        void add_param(const string& name, const string& defval, const string& description);
        
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
         * Get counter value for the given flag.
         * 
         * @param opt The character representin the flag.
         * @return int  The counter value if there's no error.
         * @return err  The error code from function execution.
         */
        multi_ret<int,err> get_counter(char opt) const;
        
        /**
         * @brief
         * Get the string value for the given parameter name.
         * 
         * @param param_name The parameter name of the value.
         * @return string The string value of the parameter.
         * @return err The error code from the function execution.
         */
        multi_ret<string,err> get_value(const string& param_name) const;
        
        /**
         * @brief
         * Checks if the flag represnted by the character is exist.
         * 
         * @return bool True if exist, false otherwise.
         */
        bool is_flag_exist(char c) const;
        
        /**
         * @brief
         * Checks of the given parameter name is exist.
         * 
         * @param param The name of the parameter.
         * @return bool True if exist, false otherwise.
         */
        bool is_param_exist(const string& param) const;

        /**
         * @brief
         * Get the translated integer value of the string command specified.
         * 
         * @return int The integer value of the specified command.
         */
        int get_command() const;

        /**
         * @brief
         * Get the failed parameter name when parsing a parameter has failed.
         * 
         * @return string The parameter name.
         */
        string get_failed_param() const;

        /**
         * @brief
         * Parse the argc and argv from `main()`.
         * 
         * @return err The status of the function execution.
         */
        err parse();

        /**
         * @brief
         * Get an argument as string given the index.
         * 
         * @param index The index of the argument.
         * @return string The argument.
         * @return err The error status fromt the function execution.
         */
        multi_ret<string,err> at(int index) const;         

        /**
         * @brief
         * Prints all flags, params and commands for the use of printing help or usage.
         */
        void print_help();

    private:
        /**
         * @brief
         * Get flag id by charcter value.
         * 
         * @param opt The character value of the flag.
         * @return int The id of the flag.
         */
        int get_flag_id(char opt) const;

        /**
         * @brief
         * Get the parameter id by the fiven parameter name.
         * 
         * @param param The parameter name.
         * @return int The id of the parameter.
         */
        int get_param_id(const string &param) const;

        /**
         * @brief
         * Parse the parameter from the string argument.
         * 
         * @param arg The string argument.
         * @return err The error status fromt the function execution. 
         */
        err parse_param(const string& arg);

        /**
         * @brief
         * Parse one or more flag(s) from the string argument
         * 
         * @param arg The string argument.
         * @return err The error status fromt the function execution. 
         */
        err parse_flag(const string& arg);

        /**
         * @brief
         * Get the mapped int value for the given command name.
         * 
         * @param name The command name.
         * @return int The integer value associated with the command.
         */
        int map_command(const string& name) const;

        /**
         * @brief
         * Get the argument count.
         * 
         * @return size_t the number of arguments including the executable. 
         */
        size_t get_size() const;

    };  // class cli_args
} // namespace

#endif // _LTD_INCLUDE_CLI_ARGS_HPP_