#include "env.hpp"

#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

#include "../inc/ltd/log.hpp"

#define PROJECT_FILE_NAME   ".project"
#define PROJECTS_SUBDIR     "projects"
#define MODULES_SUBDIR      "modules"
#define BUILDS_SUBDIR       "builds"

namespace ltd::env
{    
    multi_ret<string, env_error> read_ltd_home_var()
    {
        const char* home = std::getenv("LTD_HOME");
        if (home)
            return {string(home), env_error::no_error};
        else
            return {"", env_error::home_not_set};
    }

    multi_ret<string, env_error> get_active_project_name(const string& home_dir)
    {
        string file_name = home_dir + "/" + PROJECT_FILE_NAME;

        if(fs::exists(file_name)) {
            std::ifstream file(file_name);
            string project;

            file >> project;

            return {project, env_error::no_error};
        }
        
        return {"", env_error::missing_project_file};
    }

    file_ts get_headers_write_time(const string& dir_path)
    {
        if(!fs::exists(dir_path))
            return file_ts{};

        file_ts youngest_time;

        for(const auto& dir_entry : fs::recursive_directory_iterator(dir_path))  {
            auto ext = dir_entry.path().extension();
            if (ext != ".h" && ext != ".hpp" && ext != ".hxx") {
                continue;
            }

            file_ts entry_time;

            if (dir_entry.is_directory())
                entry_time = get_headers_write_time(dir_entry.path().string());
            else    
                entry_time = dir_entry.last_write_time();

            if (youngest_time == file_ts{} || entry_time > youngest_time) {
                youngest_time = entry_time;
            }
        }

        return youngest_time;
    }

    file_ts get_inc_write_time()
    {
        auto [project_dir, err] = get_active_project_dir();
        if(err != env_error::no_error) {
            print_env_error(err);
            return file_ts{};
        }

        string inc_dir = project_dir + "/inc";

        return get_headers_write_time(inc_dir);
    }


    void print_env_error(env::env_error err)
    {
        switch(err) {
        case env::env_error::no_error:
            log::info("No error");
            break;
        case env::env_error::home_not_set:
            log::fatal("LTD_HOME environment variable is not set");
            break;
        case env::env_error::missing_project_file:
            log::fatal("Missing .project file in home directory");
            break;
        default:
            log::fatal("Unknown error");
        }
    }    

    multi_ret<string, env_error> get_projects_dir()
    {
        auto [home_dir, err] = get_home_dir();

        if(err != env_error::no_error) {
            return {"", env_error::home_not_set};
        }

        string projects_dir = home_dir + PROJECTS_SUBDIR + "/";

        return {projects_dir, env_error::no_error};
    }

    multi_ret<string, env_error> get_modules_dir()
    {
        auto [home_dir, err] = get_home_dir();

        if(err != env_error::no_error) {
            return {"", env_error::home_not_set};
        }

        string modules_dir = home_dir + MODULES_SUBDIR + "/";

        return {modules_dir, env_error::no_error};
    }

    multi_ret<string, env_error> get_builds_dir()
    {
        auto [home_dir, err] = get_home_dir();

        if(err != env_error::no_error) {
            return {"", env_error::home_not_set};
        }

        string builds_dir = home_dir + BUILDS_SUBDIR + "/";

        return {builds_dir, env_error::no_error};
    }

    multi_ret<string, env_error> get_active_project_dir()
    {
        auto [home_dir, err] = get_home_dir();

        if(err != env_error::no_error) {
            return {"", env_error::home_not_set};
        }

        auto [project_name, project_err] = get_active_project_name();

        if(project_err != env_error::no_error) {
            return {"", env_error::missing_project_file};
        }

        string project_dir = home_dir + PROJECTS_SUBDIR + "/" + project_name + "/";

        return {project_dir, env_error::no_error};
    }

    multi_ret<string, env_error> ls_projects()
    {
        auto [projects_dir, err] = get_projects_dir();

        if(err != env_error::no_error) {
            print_env_error(err);
            return {"", err};
        }

        auto [active_project, active_err] = get_active_project_name();
        if(active_err != env_error::no_error) {
            print_env_error(active_err);
            return {"", active_err};
        }

        string listing;

        for(const auto& entry : fs::directory_iterator(projects_dir)) {
            if(entry.path().filename().string().length() == 0)
                continue;
            if (entry.path().filename().string()[0] == '.')
                continue;
            
            if(entry.path().filename().string() == active_project)
                listing += " >"; // Mark active project with an asterisk
            else
                listing += "  ";

            listing += entry.path().filename().string() + "\n";
        }

        return {listing, env_error::no_error};  
    }

    multi_ret<string, env_error> ls_modules()       
    {
        auto [modules_dir, err] = get_modules_dir();

        if(err != env_error::no_error) {
            print_env_error(err);
            return {"", err};
        }

        string listing;

        for(const auto& entry : fs::directory_iterator(modules_dir)) {
            if(entry.path().filename().string().length() == 0)
                continue;
            if (entry.path().filename().string()[0] == '.')
                continue;
            
            listing += entry.path().filename().string() + "\n";
        }

        return {listing, env_error::no_error};  
    }

    env_error set_active_project(const string& project_name)
    {
        auto [home_dir, err] = get_home_dir();

        if(err != env_error::no_error) {
            return env_error::home_not_set;
        }

        string file_name = home_dir + "/" + PROJECT_FILE_NAME;

        std::ofstream file(file_name);
        file << project_name;

        return env_error::no_error;
    }

    bool is_valid_project_name(const string& name)
    {
        // Check if the name is empty
        if (name.empty()) {
            return false;
        }

        // Check for invalid characters (you can customize this as needed)
        const string invalid_chars = "\\/:*?\"<>|";
        for (char c : name) {
            if (invalid_chars.find(c) != string::npos) {
                return false;
            }
        }

        auto [projects_dir, err] = get_projects_dir();
        if(err != env_error::no_error) {
            print_env_error(err);
            return false;
        }

        fs::path project_path = fs::path(projects_dir) / name;

        return fs::exists(project_path) && fs::is_directory(project_path);
    }
}