#include <algorithm>

#include "../inc/ltd/cli_args.hpp"
#include "../inc/ltd/fmt.hpp"

// #include "../inc/smartptr.hpp"

#include "sdk.hpp"

using namespace ltd;

void cmd_test()
{
    fmt::println("Size: %d", sizeof(int));
}

void cmd_ls()
{
    string_list dirs;
    sdk::get_projects_list(dirs);

    std::sort(dirs.begin(), dirs.end());

    string active_project = sdk::get_active_project();
    string projects_path = sdk::get_homepath() + "/projects/";

    fmt::info("Total: %d", dirs.size());
    
    for(auto dir : dirs) {
        auto timestamp = sdk::get_dir_write_time(projects_path + dir);
        auto stime = sdk::file_time_to_string(timestamp);

        if (dir == active_project)
            fmt::info("  ->%.10s %s", dir, stime);
        else
            fmt::info("    %.10s %s", dir, stime);
    }
}

void cmd_cd(cli_args& args)
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

void cmd_build(bool debug)
{
    auto active_project = sdk::get_active_project();
    
    // We need to have active project set
    if (active_project.length() == 0) {
        fmt::error("Active project is not set.");
        return;
    }

    // Iterate project dir to get build targets
    string_list dirs;
    sdk::list_project_dir(dirs);

    for (auto dir : dirs) {
        if (dir == "app") {
            sdk::build_dir(active_project, "/app", debug);
        } else if (dir == "lib") {
            sdk::build_dir(active_project, "/lib", debug);
        } else if (dir == "apps") {
            fmt::println(dir);
        } else if (dir == "libs") {
            fmt::println(dir);
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
        fmt::error("$LTD_HOME is not set.");
        return -1;
    }

    cli_args args(argc, argv);

    sdk::parse_flags(args);

    auto [v,ve] = args.get_counter('v');
    fmt::set_verbosity(fmt::INFO + v);

    fmt::debug("ltd: Verbosity level is %d", v);

    bool debug_mode = args.is_flag_exist('g');

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
        cmd_build(debug_mode);
        break;
    case sdk::CMD_CLEAN:
        cmd_clean(debug_mode);
        break;
    case sdk::CMD_TEST:
        cmd_test();
        break;
    default:
        fmt::error("ltd: Unrecognized command. See 'ltd help'.\n");
        print_usage();
        args.print_help();
    }

    return 0;
}