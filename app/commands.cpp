#include "commands.hpp"

#include "../inc/ltd/log.hpp"

namespace ltd
{
    err parse_args(cli& args, command_info& info)
    {    
        args.bind_flag(info.verbosity, 'v', "Sets verbosity level 1-4");
        args.bind_flag(info.debug_mode, 'g', "Debug mode");
        args.bind_flag(info.global, 'G', "Deploy the module globally");

        args.bind_param(info.cppstd, "std", "Specifies cpp standards");
        args.bind_param(info.imports, "imports", "List of imports, separated by colon, to link with the project");
        
        args.bind_param(info.inc_dirs, "inc-dirs", "List of include directories separated by colon");
        args.bind_param(info.inc_dirs, "lib-dirs", "List of lib directories separated by colon");
        args.bind_param(info.inc_dirs, "libs", "List of libraries separated by colon");

        args.bind_param(info.run, "run", "Specify executable to run after build");
        args.bind_param(info.run_args, "args", "Specify arguments for running executable");

        args.bind_param(info.build_target, "target", "Build target 'tests', 'app' or 'lib'.\n\t\t i.e 'ltd build --target=tests'");

        args.bind_param(info.config_name, "config", "Config name to show\n\t\t i.e 'ltd show --config=home-path'");

        args.add_command("ls",  (int)Command::ls, "List all projects in the workspace");
        args.add_command("pwd", (int)Command::pwd, "Show currect active project");
        args.add_command("cd",  (int)Command::cd, "Change project directory");
        
        args.add_command("build", (int)Command::build, "Build the current active project");
        args.add_command("clean", (int)Command::clean, "Clean the current active project");
        args.add_command("test",  (int)Command::test, "Run tests");
        args.add_command("deploy", (int)Command::deploy, "Deploy the project as importable modules.");
        args.add_command("help",  (int)Command::help, "Show this help");

        args.add_command("show", (int)Command::show, "Show some information on the screen.\n\t\t i.e 'ltd show home-path'.");

        auto err = args.parse(); 

        if (err != err::no_error || args.size() < 1)
            return err::invalid_operation; // No command provided    

        info.command = static_cast<Command>(args.get_command());

        return err;
    }
    
    void print_usage()
    {
        cli::println("Usage: ltd <command> [-vgG] [<args>]\n");
    }
}