#ifndef _LTD_ENV_HPP_
#define _LTD_ENV_HPP_

#include "../inc/ltd/stddef.hpp"
#include "../inc/ltd/stdltd.hpp"

namespace ltd
{
    namespace env
    {
        enum class env_error
        {
            no_error,
            home_not_set,
            missing_project_file,
            missing_dir
        };

        /**
         * Reads the home directory from the environment variable "LTD_HOME". 
         * If the variable is not set, returns an error.
         * 
         * @return A tuple containing the home directory as a string and an 
         *         env_error indicating success or failure.
         */
        multi_ret<string, env_error> read_ltd_home_var();

        /**
         * Gets the active project name from the ".project" file in the home directory. 
         * If the home directory is not set or the ".project" file is missing, returns an error.
         * 
         * @return A tuple containing the active project name as a string and an 
         *         env_error indicating success or failure.
         */
        multi_ret<string, env_error> get_active_project_name(const string& home_dir);

        /**
         * @brief
         * Recursively get the youngest write time of header files in the given 
         * directory.
         * 
         * @param dir_path The path of the directory to search for header files.
         * @return The youngest write time of header files in the directory, or an 
         *         empty file_ts if the directory does not exist or an error occurs.
         */
        file_ts get_headers_write_time(const string& dir_path);

        /**
         * @brief
         * Get the write time of the youngest header file in the "inc" directory 
         * of the active project.
         * 
         * @return The write time of the youngest header file, or an empty 
         *         file_ts if the inc folder does not exist or an error occurs.
         */
        file_ts get_inc_write_time();

        /**
         * @brief
         * Print the error message corresponding to the given env_error.
         * 
         * @param err The env_error to print the message for.
         */
        void print_env_error(env::env_error err);        

        /**
         * Gets the projects directory path by appending "/projects/" to the home directory. 
         * If the home directory is not set, returns an error.
         * 
         * @return A tuple containing the projects directory path as a string and an 
         *         env_error indicating success or failure.
         */
        multi_ret<string, env_error> get_projects_dir();

        /**
         * Gets the modules directory path by appending "/modules/" to the home directory. 
         * If the home directory is not set, returns an error.
         * 
         * @return A tuple containing the modules directory path as a string and an 
         *         env_error indicating success or failure.
         */
        multi_ret<string, env_error> get_modules_dir();

        /**
         * Gets the builds directory path by appending "/builds/" to the home directory. 
         * If the home directory is not set, returns an error.
         * 
         * @return A tuple containing the builds directory path as a string and an 
         *         env_error indicating success or failure.
         */
        multi_ret<string, env_error> get_builds_dir();

        /**
         * @brief
         * Get the active project directory path by combining the home directory and the active project name.
         * If the home directory is not set or the active project name cannot be retrieved, returns an error.
         * 
         * @return A tuple containing the active project directory path as a string and an 
         *         env_error indicating success or failure.
         */  
        multi_ret<string, env_error> get_active_project_dir();

        /**
         * @brief
         * List the projects in the projects directory.
         * If the home directory is not set or the projects directory cannot be accessed, returns an error.
         * 
         * @return A tuple containing the listing of projects as a string and an 
         *         env_error indicating success or failure.
         */
        multi_ret<string, env_error> ls_projects();

        /**
         * @brief
         * List the modules in the modules directory.
         * If the home directory is not set or the modules directory cannot be accessed, returns an error.
         * 
         * @return A tuple containing the listing of modules as a string and an 
         *         env_error indicating success or failure.
         */
        multi_ret<string, env_error> ls_modules();

        /**
         * @brief
         * Set the active project name in the ".project" file.
         * 
         * @param project_name The name of the project to set as active.
         * @return An env_error indicating success or failure.
         */
        env_error set_active_project(const string& project_name);

        /**
         * @brief
         * Check if a project name is valid.
         * 
         * @param project_name The name of the project to check.
         * @return True if the project name is valid, false otherwise.
         */
        bool is_valid_project_name(const string& project_name);

        multi_ret<string, env_error> get_builds_dir();
    }
}

#endif // _LTD_ENV_HPP_