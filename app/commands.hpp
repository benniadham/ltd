#ifndef _LTD_COMMANDS_HPP_
#define _LTD_COMMANDS_HPP_

#include "../inc/ltd/cli.hpp"

namespace ltd
{
    enum class Command
    {
        ls,
        pwd,
        cd,
        build,
        clean,
        test,
        deploy,
        help,
        show,
        run
    };

    struct command_info
    {
        int verbosity   = 0;
        int debug_mode  = 0;
        int global      = 0;

        string cppstd   = "c++20";
        string run;
        string run_args;
        string build_target;
        
        string_list imports;
        string_list inc_dirs;
        string_list lib_dirs;
        string_list libs;

        string config_name;         // For show information command

        Command command;        
    };
        
    /**
     * @brief Parse command line arguments and populate the command_info structure.
     * 
     * @param args The command line arguments.
     * @param info The command_info structure to populate.
     * @return An error code indicating success or failure.
     */
    err parse_args(cli& args, command_info& info);

    /**
     * @brief Print the usage information for the command-line interface.
     */
    void print_usage();
}

#endif // _LTD_COMMANDS_HPP_