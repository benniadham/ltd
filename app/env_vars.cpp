#include "env_vars.hpp"

#include <fstream>

#include "../inc/ltd/stddef.hpp"
#include "../inc/ltd/stdfs.hpp"

#define LTD_HOME_VAR        "LTD_HOME"
#define PROJECT_FILE_NAME   ".project"

namespace ltd
{
    env_vars::env_vars()
    {
        const char* ltd_home_var = std::getenv(LTD_HOME_VAR);
        if (ltd_home_var != nullptr) 
        {
            home_dir = string(ltd_home_var);
            home_var_exists = true;
        
            file_name = home_dir + "/" + PROJECT_FILE_NAME;
            if (fs::exists(file_name)) 
            {
                active_project_var_exists = true;
                std::ifstream file(file_name);
                file >> active_project;
                file.close();
            }        
        }        
    }

    bool env_vars::test_home_var() const
    {
        return home_var_exists && fs::exists(home_dir) && fs::is_directory(home_dir);
    }

    bool env_vars::test_active_project_var() const
    {
        return active_project_var_exists && !active_project.empty();
    }

    string env_vars::get_home_var() const
    {
        return home_dir;
    }

    string env_vars::get_active_project_var() const
    {
        return active_project;
    }

    bool env_vars::set_active_project_var(const string& project_name)
    {
        std::ofstream file(file_name);
        if (!file.is_open()) {
            return false;
        }
        file << project_name;
        file.close();
        active_project = project_name;
        active_project_var_exists = true;
        return true;
    }
}