#include "ltd_home.hpp"

#include <chrono>
#include <cassert>
#include "../inc/ltd/log.hpp"
#include "../inc/ltd/stdfs.hpp"

#define PROJECTS_SUBDIR     "projects"
#define MODULES_SUBDIR      "modules"
#define BUILDS_SUBDIR       "builds"
#define INC_SUBDIR          "/inc"

ltd::ltd_home the_home;

namespace ltd   
{
    bool ltd_home::singleton_flag=true;

    ltd_home::ltd_home() 
    {        
        assert(singleton_flag && "ltd_home is a singleton class. Only one instance should be created.");
        singleton_flag = false;
    }

    bool ltd_home::is_ltd_home_set() const
    {
        return vars.test_home_var();
    }

    bool ltd_home::is_active_project_set() const
    {
        return vars.test_active_project_var();
    }

    string ltd_home::get_home_dir() const
    {
        return vars.get_home_var();
    }

    string ltd_home::get_active_project_name() const
    {
        return vars.get_active_project_var();
    }

    bool ltd_home::set_active_project(const string& project_name)
    {
        return vars.set_active_project_var(project_name);
    }

    bool ltd_home::is_valid_project_name(const string& name) const
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

        auto projects_dir = get_projects_dir();
        
        fs::path project_path = fs::path(projects_dir) / name;

        return fs::exists(project_path) && fs::is_directory(project_path);
    }

    string ltd_home::get_projects_dir() const
    {
        return get_home_dir() + PROJECTS_SUBDIR;
    }

    string ltd_home::get_project_dir(const string& name) const
    {
        return get_projects_dir() + "/" + name;
    }

    string ltd_home::get_project_inc_dir(const string& name) const
    {
        return get_project_dir(name) + INC_SUBDIR;
    }

    string ltd_home::get_active_project_dir() const
    {
        return get_project_dir(get_active_project_name());
    }

    string ltd_home::get_active_project_inc_dir() const
    {
        return get_active_project_dir() + INC_SUBDIR;
    }

    string ltd_home::get_builds_dir() const
    {
        return get_home_dir() + BUILDS_SUBDIR;
    }

    string ltd_home::get_build_dir(const string& name, bool debug) const
    {
        return get_builds_dir() + "/" + name + "/" + (debug ? "debug" : "release");
    }

    string ltd_home::get_build_subdir(const string& name, bool debug, const string& subdir) const
    {
        return get_build_dir(name, debug) + "/" + subdir;
    }

    string ltd_home::get_active_build_dir(bool debug) const
    {
        return get_build_dir(get_active_project_name(), debug);
    }    
    
    string ltd_home::get_active_build_subdir(bool debug, const string& subdir) const
    {
        return get_build_subdir(get_active_project_name(), debug, subdir);
    }

    string ltd_home::get_modules_dir() const
    {
        return get_home_dir() + MODULES_SUBDIR;
    }

    string ltd_home::get_module_dir(const string& module) const
    {
        return get_modules_dir() + "/" + module;
    }

    string ltd_home::get_module_inc_dir(const string& name) const
    {
        return get_module_dir(name) + INC_SUBDIR;
    }

    string ltd_home::get_test_dir() const
    {
        return get_active_project_dir() + "/tests";
    }   

    bool ltd_home::get_libs(const string& dir, string_list& libs) const
    {
        log::trace("Looking for libraries under directory: %s", dir);

        if (!fs::exists(dir)) {
            log::error("Directory does not exist: %s", dir);
            return false;
        }

        for (const auto& entry : fs::directory_iterator(dir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".a") {
                auto filename = entry.path().filename().string();
                filename = filename.substr(3, filename.size() - 5); // Remove 'lib' prefix and '.a' suffix
                log::trace("Found library: %s", filename);
                libs.push_back(filename);
            }            
        }
        
        return true;
    }

    file_ts ltd_home::get_headers_write_time(const string& dir_path) const
    {
        static std::chrono::system_clock::time_point timeout_1970{std::chrono::seconds(0)};
        static file_ts file_time = std::chrono::clock_cast<std::chrono::file_clock>(timeout_1970);

        if(!fs::exists(dir_path))
            return file_time;
        
        file_ts youngest_time = file_time;

        for(const auto& dir_entry : fs::recursive_directory_iterator(dir_path))  {
            auto ext = dir_entry.path().extension();
            if (ext != ".h" && ext != ".hpp" && ext != ".hxx") {
                continue;
            }

            file_ts entry_time = file_time;

            if (dir_entry.is_directory())
                entry_time = get_headers_write_time(dir_entry.path().string());
            else    
                entry_time = dir_entry.last_write_time();

            if (youngest_time == file_time || entry_time > youngest_time) {
                youngest_time = entry_time;
            }
        }

        return youngest_time;
    }

    file_ts ltd_home::read_lib_header_ts() const
    {
        auto inc_dir = get_active_project_inc_dir();
        return get_headers_write_time(inc_dir);
    }

    string ltd_home::list_projects() const
    {
        string listing;

        auto projects_dir = get_projects_dir();

        for(const auto& entry : fs::directory_iterator(projects_dir)) {
            if(entry.is_directory()==false)
                continue;
            if(entry.path().filename().string().length() == 0)
                continue;
            if (entry.path().filename().string()[0] == '.')
                continue;
            
            if(entry.path().filename().string() == get_active_project_name())
                listing += " >"; // Mark active project with an asterisk
            else
                listing += "  ";
            
            listing += entry.path().filename().string() + "\n";
        }

        return listing;
    }

    string ltd_home::list_modules() const
    {
        string listing;

        auto modules_dir = get_modules_dir();

        for(const auto& entry : fs::directory_iterator(modules_dir)) {
            if(entry.is_directory()==false)
                continue;
            if(entry.path().filename().string().length() == 0)
                continue;
            if (entry.path().filename().string()[0] == '.')
                continue;
            
            listing += entry.path().filename().string() + "\n";
        }

        return listing;
    }

    bool ltd_home::clear_active_project(bool debug)
    {
        auto active_project = get_active_project_name();
        if (active_project.empty()) {
            log::error("No active project is set.");
            return false;
        }

        return clear_project(active_project, debug);
    }

    bool ltd_home::clear_project(const string& project_name, bool debug)
    {
        auto build_dir = get_build_dir(project_name, debug);
        if (!fs::exists(build_dir)) {
            log::error("Build directory does not exist: %s", build_dir);
            return false;
        }

        try {
            fs::remove_all(build_dir);
            log::info("Cleared build directory: %s", build_dir);
            return true;
        } catch (const fs::filesystem_error& e) {
            log::error("Failed to clear build directory: %s. Error: %s", build_dir, e.what());
            return false;
        }
    }
}