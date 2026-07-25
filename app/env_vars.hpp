#ifndef _LTD_ENV_VARS_HPP_
#define _LTD_ENV_VARS_HPP_

#include "../inc/ltd/stddef.hpp"
#include "../inc/ltd/stdltd.hpp"

namespace ltd
{
    /**
     * @brief 
     * Class to manage environment variables related to the LTD project.
     */
    class env_vars
    {
    public:
        /**
         * @brief Construct a new env_vars object and initialize it by reading the environment variables. 
         */
        env_vars();

        /**
         * @brief Test if the LTD_HOME environment variable is set and points to a valid directory.
         * 
         * @return true if the LTD_HOME variable is set and points to a valid directory, false otherwise.
         */
        bool test_home_var() const;

        /**
         * @brief Test if the active project environment variable is set and not empty.
         * 
         * @return true if the active project variable is set and not empty, false otherwise.
         */
        bool test_active_project_var() const;

        /**
         * @brief Get the value of the LTD_HOME environment variable.
         * 
         * @return The value of the LTD_HOME environment variable.
         */
        string get_home_var() const;

        /**
         * @brief Get the value of the active project environment variable.
         * 
         * @return The value of the active project environment variable.
         */
        string get_active_project_var() const;

        /**
         * @brief Set the value of the active project environment variable.
         * 
         * @param project_name The name of the project to set as active.
         * @return true if the operation was successful, false otherwise.
         */
        bool set_active_project_var(const string& project_name);        
        
    private:
        string home_dir;
        string file_name;
        string active_project;
        bool home_var_exists=false;
        bool active_project_var_exists=false;
    };
}

#endif // _LTD_ENV_VARS_HPP_