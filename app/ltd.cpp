#include <algorithm>
#include <iostream>
#include <variant>

#include "../inc/ltd/cli.hpp"
#include "../inc/ltd/fmt.hpp"
#include "../inc/ltd/stddef.hpp"

#include "sdk.hpp"

using namespace ltd;

void cmd_deploy(int global)
{
    sdk::deploy_to_module_path();
}

void cmd_ls()
{
    string_list dirs;
    sdk::get_projects_list(dirs);

    std::sort(dirs.begin(), dirs.end());

    string active_project = sdk::get_active_project();
    string projects_path = sdk::get_homepath() + "/projects/";

    cli::info("Total: %d", dirs.size());
    
    for(auto dir : dirs) {
        auto timestamp = sdk::get_dir_write_time(projects_path + dir);
        auto stime = sdk::file_time_to_string(timestamp);

        if (dir == active_project)
            cli::info("  ->%.10s %s", dir, stime);
        else
            cli::info("    %.10s %s", dir, stime);
    }
}

void cmd_cd(cli& args)
{
    auto [select_project, e] = args.at(1);

    if (e != err::no_error)
        return;
        
    fmt::println(select_project);

    if (select_project.length() > 0)
        sdk::set_active_project(select_project);
}

void cmd_pwd()
{
    fmt::println(sdk::get_active_project());
}

void cmd_build(bool debug, string_list& imports)
{
    auto active_project = sdk::get_active_project();
    
    // We need to have active project set
    if (active_project.length() == 0) {
        cli::error("Active project is not set.");
        return;
    }

    // Iterate project dir to get build targets
    string_list dirs;
    sdk::list_project_dir(dirs);

    for (auto dir : dirs) {
        if (dir == "app") {
            sdk::build_dir(active_project, "/app", debug, imports);
        } else if (dir == "lib") {
            sdk::build_dir(active_project, "/lib", debug, imports);
        } else if (dir == "apps") {
            cli::fatal("Needs to implement apps");
            // exit(-1);
        } else if (dir == "libs") {
            cli::fatal("Needs to implement libs");
            // exit(-1);
        }
    }
}

void cmd_clean(bool debug) 
{
    sdk::clean_project(debug);
}

void print_usage()
{
    fmt::println("Usage: ltd [ -v | --verbosity=n ] [ -h ] <commands]> [<args>]\n");
}

auto main(int argc, char *argv[]) -> int {
    
    if (sdk::is_homepath_set() == false) {
        cli::error("$LTD_HOME is not set.");
        return -1;
    }

    // cli_args args(argc, argv);

    // sdk::parse_flags(args);

    // auto [v,ve] = args.get_counter('v');
    // fmt::set_verbosity(fmt::INFO + v);

    // fmt::debug("ltd: Verbosity level is %d", v);

    // bool debug_mode = args.is_flag_exist('g');

    cli args(argc, argv);

    int verbosity   = 0;
    int debug_mode  = 0;
    int global      = 0;

    string cppstd;
    
    string_list imports;

    args.bind_flag(verbosity, 'v', "Sets verbosity level 1-4");
    args.bind_flag(debug_mode, 'g', "Debug mode");
    args.bind_flag(global, 'G', "Deploy the module globally");

    args.bind_param(cppstd, "std", "Specifies cpp standards");
    args.bind_param(imports, "imports", "List of imports to link with the project");

    args.add_command("ls",  sdk::CMD_LS, "List all projects in the workspace");
    args.add_command("pwd", sdk::CMD_PWD, "Show currect active project");
    args.add_command("cd",  sdk::CMD_CD, "Change project directory");
    
    args.add_command("build", sdk::CMD_BUILD, "Build the current active project");
    args.add_command("clean", sdk::CMD_CLEAN, "Clean the current active project");
    args.add_command("test",  sdk::CMD_TEST, "Run tests");
    args.add_command("deploy", sdk::CMD_DEPLOY, "Deploy the project as importable modules.");
    args.add_command("help",  sdk::CMD_HELP, "Show this help");

    args.parse();

    cli::set_log_level(verbosity + cli::LOG_INFO);
    
    switch(args.get_command())
    {
    case sdk::CMD_LS:
        cmd_ls();
        break;
    case sdk::CMD_PWD:
        cmd_pwd();
        break;
    case sdk::CMD_CD:
        cmd_cd(args);
        break;
    case sdk::CMD_BUILD:
        cmd_build(debug_mode, imports);
        break;
    case sdk::CMD_CLEAN:
        cmd_clean(debug_mode);
        break;
    case sdk::CMD_TEST:
        break;
    case sdk::CMD_DEPLOY:
        cmd_deploy(global);
        break;
    default:
        cli::error("ltd: Unrecognized command. See 'ltd help'.\n");
        print_usage();
        args.print_help();
    }

    return 0;
}