#include "command_handlers.hpp"

#include "../inc/ltd/log.hpp"

namespace ltd
{
    void show_config(const string& config_name)
    {
        log::debug("Showing config: %s", config_name.c_str());

        if(config_name == "home-dir") 
        {
            cli::println(the_home.get_home_dir());
        } 
        else if(config_name == "projects-dir") 
        {
            cli::println(the_home.get_projects_dir());
        } 
        else if(config_name == "modules-dir") 
        {
            cli::println(the_home.get_modules_dir());
        } 
        else if(config_name == "builds-dir") 
        {
            cli::println(the_home.get_builds_dir());
        } 
        else if(config_name == "active-project") 
        {
            cli::println(the_home.get_active_project_name());
        } 
        else if(config_name == "active-project-dir") 
        {
            cli::println(the_home.get_active_project_dir());
        } 
        else if(config_name == "projects") 
        {
            cli::printf("%s", the_home.list_projects());
        } 
        else if(config_name == "modules") 
        {
            cli::printf("%s", the_home.list_modules());
        } 
        else if (config_name == "?") 
        {
            cli::println("Available option:\n"
                        "  home-dir, projects-dir, modules-dir,\n"
                        "  builds-dir, active-project, active-project-dir,\n"
                        "  projects, modules");
        } 
        else 
        {
            log::error("Unrecognized config name: '%s'", config_name.c_str());

            cli::println("Available option:\n"
                        "  home-dir, projects-dir, modules-dir,\n"
                        "  builds-dir, active-project, active-project-dir,\n"
                        "  projects, modules");
        }
    }

    bool ls()   {    
        cli::printf("%s", the_home.list_projects());
        return true;
    }
}