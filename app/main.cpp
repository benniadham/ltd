#include "../inc/ltd/cli.hpp"
#include "../inc/ltd/fmt.hpp"
#include "../inc/ltd/log.hpp"
#include "../inc/ltd/stddef.hpp"
#include "../inc/ltd/stdfs.hpp"

#include "commands.hpp"
#include "command_handlers.hpp"
#include "build_tools.hpp"
#include "ltd_home.hpp"

using namespace ltd;

auto main(int argc, char* argv[]) -> int
{
    // Initialize the logger
    log::init_console_logger();

    // Parse cli arguments 
    cli args(argc, argv);    
    command_info cmd_info;

    auto err = parse_args(args, cmd_info);

    if(err != err::no_error) {
        log::error("Failed to parse command line arguments");
        return 1;
    }

    // Set the log verbosity level
    log::set_verbosity(cmd_info.verbosity);

    if(the_home.is_ltd_home_set() == false) {
        log::fatal("LTD_HOME environment variable is not set or the directory does not exist.");
        return -1;   
    }   

    if(the_home.is_active_project_set()) {
        log::debug("Active project is set to '%s'", the_home.get_active_project_name());
        log::debug("Active project dir is '%s'", the_home.get_active_project_dir());
    }        

    switch(cmd_info.command)
    {
    case Command::ls:
        if(the_home.is_active_project_set() == false) {
            log::fatal("No active project is set. Please set an active project using the 'cd' command.");
            return -1;
        }
        ls();
        break;
    case Command::cd:
        {
            auto [project, err] = args.at(1);
            if(err != err::no_error) {
                log::fatal("Expected project name after 'cd' command");
                return -1;   
            }

            bool project_exists = the_home.is_valid_project_name(project);
            if(!project_exists) {
                log::fatal("Bad project name or project '%s' does not exist", project);
                return -1;
            }

            auto set_err = the_home.set_active_project(project);
            if(set_err == false) {
                log::fatal("Failed to set active project: %s", project);
                return -1;
            }

            ls(); // Show the new active project after changing it   
        }
        break;
    case Command::pwd:
        {
            if(the_home.is_active_project_set() == false) {
                log::fatal("No active project is set. Please set an active project using the 'cd' command.");
                return -1;
            }
            cli::println(the_home.get_active_project_dir());            
        }
        break;
    case Command::build:
        {
            build_tools tools;
            tools.configure(cmd_info);
            
            log::debug("Active build dir is '%s'", the_home.get_active_build_dir(cmd_info.debug_mode > 0));

            if (!fs::exists(the_home.get_active_build_dir(cmd_info.debug_mode > 0))) {
                log::warn("Active build directory does not exist: %s", the_home.get_active_build_dir(cmd_info.debug_mode > 0));
                log::info("Creating active build directory: %s", the_home.get_active_build_dir(cmd_info.debug_mode > 0));
                fs::create_directories(the_home.get_active_build_dir(cmd_info.debug_mode > 0));
            }            

            if (cmd_info.build_target.empty() || cmd_info.build_target == "all") 
            {
                tools.build_libs();
                tools.build_apps();
                tools.build_tests();
            } 
            else if (cmd_info.build_target == "lib") 
            {
                auto result = tools.build_lib("lib", the_home.get_active_project_name());
                if(result == false) {
                    log::fatal("Failed to build target: %s", cmd_info.build_target);
                    return -1;
                }
            } 
            else if (cmd_info.build_target == "app") 
            {
                // Build libraries first, then build the app
                auto result = tools.build_app("app", the_home.get_active_project_name());
                if(result == false) {
                    log::fatal("Failed to build target: %s", cmd_info.build_target);
                    return -1;
                }
            } 
            else if (cmd_info.build_target == "test" || cmd_info.build_target == "tests") 
            {
                tools.build_libs();
                // Build libraries first, then build the tests
                auto result = tools.build_tests();
                if(result == false) {
                    log::fatal("Failed to build target: %s", cmd_info.build_target);
                    return -1;
                }
            } 
            else if(cmd_info.build_target == "libs") 
            {
                tools.build_libs();
            }
            else if(starts_with(cmd_info.build_target, "libs/")) 
            {
                string sub_dir = the_home.get_active_project_dir() + "/" + cmd_info.build_target;
                log::trace("Building library in subdirectory: %s", sub_dir);
                if(!fs::exists(sub_dir)) {
                    log::fatal("Subdirectory does not exist: %s", cmd_info.build_target);
                    return -1;
                }
                auto name = cmd_info.build_target.substr(5); // Remove "libs/" prefix
                if(name.empty()) {
                    log::fatal("Library name is empty. Please specify a library name after 'libs/'");
                    return -1;
                }
                auto result = tools.build_lib(cmd_info.build_target, name);
                if(result == false) {
                    log::fatal("Failed to build target: %s", cmd_info.build_target);
                    return -1;
                }
            } 
            else if(starts_with(cmd_info.build_target, "apps/")) 
            {
                string sub_dir = the_home.get_active_project_dir() + "/" + cmd_info.build_target;
                log::trace("Building application in subdirectory: %s", sub_dir);
                if(!fs::exists(sub_dir)) {
                    log::fatal("Subdirectory does not exist: %s", cmd_info.build_target);
                    return -1;
                }
                auto name = cmd_info.build_target.substr(5); // Remove "apps/" prefix
                if(name.empty()) {
                    log::fatal("Application name is empty. Please specify an application name after 'apps/'");
                    return -1;
                }
                auto result = tools.build_app(cmd_info.build_target, name);
                if(result == false) {
                    log::fatal("Failed to build target: %s", cmd_info.build_target);
                    return -1;
                }
            }
            else if (cmd_info.build_target == "apps") 
            {
                tools.build_libs();
                tools.build_apps();
            } 
            else 
            {
                log::fatal("Unknown build target: %s", cmd_info.build_target);
                return 1;
            } 
        }
        break;
    case Command::clean:
        {
            the_home.clear_active_project(cmd_info.debug_mode > 0);
        }
        break;
    case Command::test:
        log::info("Command: test");
        {
            if(cmd_info.build_target.empty() || cmd_info.build_target == "tests" || cmd_info.build_target == "all") {
                auto active_project = the_home.get_active_project_name();
                if(active_project.empty()) {
                    log::fatal("No active project is set. Please set an active project using the 'cd' command.");
                    return -1;  
                }
                auto test_dir = the_home.get_active_build_dir(cmd_info.debug_mode > 0) + "/tests";
                if(!fs::exists(test_dir)) {
                    log::fatal("Test directory does not exist: %s", test_dir);
                    return -1;
                }
                for(const auto& dir_entry : fs::directory_iterator(test_dir)) {
                    if (dir_entry.is_directory())
                        continue;
                    if (dir_entry.path().extension() == ".o")
                        continue;
                    
                    auto filename = dir_entry.path().filename().replace_extension("");
                    cli::printf("Running unit test %-13s ........................ ", filename);
                    cli::flush();

                    auto exec = dir_entry.path();
                    exec += " -a";
                    auto res = std::system(exec.c_str());
                    if (res != 0) {
                        cli::println("FAILED (exit code: %d)", res);
                    }
                }
            } 
            else
            {
                auto test_file = the_home.get_active_build_dir(cmd_info.debug_mode > 0) + "/tests/" + cmd_info.build_target;
                if(!fs::exists(test_file)) {
                    log::fatal("Test file does not exist: %s", test_file);
                    return -1;
                }
                log::info("Running test: %s", test_file);
                auto result = std::system((test_file + " " + cmd_info.run_args).c_str());
                if(result != 0) {
                    log::fatal("Test failed with exit code: %d", result);
                    return -1;  
                }
            }            
        }
        break;
    case Command::deploy:
        log::info("Command: deploy");
        {
            // Copy the built files to the modules directory
            auto active_project = the_home.get_active_project_name();
            if(active_project.empty()) {
                log::fatal("No active project is set. Please set an active project using the 'cd' command.");
                return -1;  
            }
            auto target_dir = the_home.get_module_dir(active_project);
            if(!fs::exists(target_dir)) {
                log::trace("Creating module directory: %s", target_dir);
                if(!fs::create_directories(target_dir)) {
                    log::fatal("Failed to create module directory: %s", target_dir);
                    return -1;
                }
            }
            auto build_dir = the_home.get_active_build_dir(false);
            if(!fs::exists(build_dir)) {
                log::fatal("Active build directory does not exist: %s", build_dir);
                return -1;
            }

            auto include_dir = the_home.get_active_project_dir() + "/inc";
            
            log::trace("Header: %s", include_dir);
            log::trace("Binary: %s", build_dir);
            log::trace("Target: %s", target_dir);

            if(fs::exists(include_dir)) {
                log::info("Copying include directory to module directory");
                fs::copy(include_dir, target_dir + "/inc", fs::copy_options::recursive | fs::copy_options::overwrite_existing);
            } else {
                log::warn("Include directory does not exist: %s", include_dir); 
            }

            log::info("Copying build directory to module directory");
            for(const auto& dir_entry : fs::directory_iterator(build_dir)) {
                if (dir_entry.is_directory()) 
                    continue;
                if (dir_entry.is_regular_file() == false)
                    continue;
                if (dir_entry.path().filename().has_extension() && dir_entry.path().filename().extension() != ".a")
                    continue;
                log::trace("Copying file: %s", dir_entry.path().filename().string());
                fs::copy(dir_entry.path(), target_dir + "/" + dir_entry.path().filename().string(), fs::copy_options::overwrite_existing);
            }
        }
        break;
    case Command::help:
        print_usage();
        args.print_help();
        break;
    case Command::show:
        log::info("Command: show");
        {
            auto [config, err] = args.at(1);
            if(err != err::no_error && (cmd_info.config_name.empty() || config.at(0) == '-')) {
                log::error("Expected config name after 'show' command");
                return 1;
            }
            show_config(config);            
        }        
        break;
    case Command::run:
        log::info("Command: run");
        {
            auto run_file = the_home.get_active_build_dir(cmd_info.debug_mode > 0) + "/" + cmd_info.run;
            if(!fs::exists(run_file)) {
                log::fatal("Run file does not exist: %s", run_file);
                return -1;
            }
            log::info("Running executable: %s", run_file);
            auto result = std::system((run_file + " " + cmd_info.run_args).c_str());
            if(result != 0) {
                log::fatal("Executable failed with exit code: %d", result);
                return -1;  
            }
        }
        break;
    default:
        log::error("Unknown command");
        break;
    }

    return 0;
}
    