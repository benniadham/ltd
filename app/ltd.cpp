#include "../inc/flags.hpp"
#include "../inc/fmt.hpp"

#include "sdk.hpp"

using namespace ltd;

void cmd_ls()
{
    StringList dirs;
    sdk::get_projects_list(dirs);

    std::sort(dirs.begin(), dirs.end());

    String active_project = sdk::get_active_project();
    String projects_path = sdk::get_homepath() + "/projects/";

    fmt::println("Total: %d", dirs.size());
    for(auto dir : dirs) {
        
        auto timestamp = sdk::get_dir_write_time(projects_path + dir);
        auto stime = sdk::file_time_to_string(timestamp);

        if (dir == active_project)
            fmt::println("  ->%.10s %s", dir, stime);
        else
            fmt::println("    %.10s %s", dir, stime);
    }
}

void cmd_cd(Flags& flags)
{
    String select_project = flags.at(1);

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
        fmt::println("Active project is not set.");
        return;
    }

    // Iterate project dir to get build targets
    StringList dirs;
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

auto main(int argc, char *argv[]) -> int {
    
    if (sdk::is_homepath_set() == false) {
        std::cout << "$LTD_HOME is not set." << std::endl;
        return -1;
    }

    Flags flag(argc, argv);

    sdk::parse_flags(flag);

    switch(flag.get_cmd())
    {
    case sdk::CMD_LS:
        cmd_ls();
        break;
    case sdk::CMD_PWD:
        cmd_pwd();
        break;
    case sdk::CMD_CD:
        cmd_cd(flag);
        break;
    case sdk::CMD_BUILD:
        cmd_build(flag.is_exist('g'));
        break;
    case sdk::CMD_CLEAN:
        cmd_clean(flag.is_exist('g'));
        break;
    default:
        fmt::println("Unrecognized command");
    }

    return 0;
}