#ifndef _LTD_HOME_HPP_
#define _LTD_HOME_HPP_

#include "../inc/ltd/stddef.hpp"
#include "../inc/ltd/stdfs.hpp"
#include "../inc/ltd/stdltd.hpp"

#include "env_vars.hpp"

namespace ltd
{  
    /**
     * The ltd_home class represents the home directory of the ltd project. 
     * It provides functionality to initialize the home directory and check 
     * for necessary files.
     */
    class ltd_home  
    {
    public:
        /**
         * Constructor for the ltd_home class. Initializes the home directory and checks for necessary files.
         */
        ltd_home();

        /**
         * @brief
         * Checks if the LTD_HOME environment variable is set and points to a valid directory.
         * 
         * @return True if the LTD_HOME environment variable is set and valid, false otherwise.
         */
        bool is_ltd_home_set() const;

        /**
         * @brief
         * Checks if an active project is set in the environment variables.
         * 
         * @return True if an active project is set, false otherwise.
         */
        bool is_active_project_set() const;

        /**
         * @brief
         * Getter for the home directory path.
         * 
         * @return The path to the home directory as a string.
         */
        string get_home_dir() const;

        /**
         * @brief
         * Getter for the active project name.
         * 
         * @return The name of the active project as a string.
         */
        string get_active_project_name() const;

        /**
         * @brief
         * Sets the active project in the environment variables.
         * 
         * @param project_name The name of the project to set as active.
         * @return True if the active project was successfully set, false otherwise.
         */
        bool set_active_project(const string& project_name);

        /**
         * @brief
         * Checks if a given project name is valid.
         * 
         * @param name The name of the project to check.
         * @return True if the project name is valid, false otherwise.
         */
        bool is_valid_project_name(const string& name) const;

        /**
         * @brief
         * Getter for the projects directory path under the LTD_HOME directory.
         * 
         * @return The path to the projects directory as a string.
         */
        string get_projects_dir() const;

        /**
         * @brief
         * Getter for the project directory path of a specific project.
         * 
         * @param name The name of the project.
         * @return The path to the project's directory as a string.
         */
        string get_project_dir(const string& name) const;

        /**
         * @brief
         * Getter for the include directory path of a specific project.
         * 
         * @param name The name of the project.
         * @return The path to the project's include directory as a string.
         */
        string get_project_inc_dir(const string& name) const;        
        
        /**
         * @brief
         * Getter for the active project directory path.
         * 
         * @return The path to the active project directory as a string.
         */
        string get_active_project_dir() const;        
        
        /**
         * @brief   
         * Getter for the active project's include directory path.
         * 
         * @return The path to the active project's include directory as a string.
         */
        string get_active_project_inc_dir() const;

        /**
         * @brief
         * Getter for the builds directory path under the LTD_HOME directory.
         * 
         * @return The path to the builds directory as a string.
         */
        string get_builds_dir() const;

        /**
         * @brief
         * Getter for the build directory path of a specific project.
         * 
         * @param name The name of the project.
         * @param debug A boolean indicating whether to get the debug or release build directory.
         * @return The path to the project's build directory as a string.
         */
        string get_build_dir(const string& name, bool debug) const;

        /**
         * @brief
         * Getter for a specific subdirectory within the build directory of a specific project.
         * 
         * @param name The name of the project.
         * @param debug A boolean indicating whether to get the debug or release build directory.
         * @param subdir The name of the subdirectory to get.
         * @return The path to the specified subdirectory within the project's build directory as a string.
         */
        string get_build_subdir(const string& name, bool debug, const string& subdir) const;

        /**
         * @brief
         * Getter for the active builds directory path.
         *
         * @return The path to the active builds directory as a string.
         */
        string get_active_build_dir(bool debug) const;

        /**
         * @brief
         * Getter for a specific subdirectory within the active build directory.
         *
         * @param debug A boolean indicating whether to get the debug or release build directory.
         * @param subdir The name of the subdirectory to get.
         * @return The path to the specified subdirectory within the active build directory as a string.
         */
        string get_active_build_subdir(bool debug, const string& subdir) const;

        /**
         * @brief
         * Getter for the modules directory path under the LTD_HOME directory.
         * 
         * @return The path to the modules directory as a string.
         */
        string get_modules_dir() const;

        /**
         * @brief
         * Getter for the module directory path of a specific module.
         * 
         * @param name The name of the module.
         * @return The path to the module's directory as a string.
         */
        string get_module_dir(const string& module) const;

        /**
         * @brief
         * Getter for the include directory path of a specific module.
         * 
         * @param name The name of the module.
         * @return The path to the module's include directory as a string.
         */
        string get_module_inc_dir(const string& name) const;

        /**
         * @brief
         * Getter for the tests directory path under the LTD_HOME directory.
         * 
         * @return The path to the tests directory as a string.
         */
        string get_test_dir() const;

        /**
         * @brief
         * Reads the write time of the library header file.
         * 
         * @return The write time of the library header file as a file_ts object.
         */
        file_ts read_lib_header_ts() const;
        
        /**
         * @brief
         * Retrieves the list of libraries present in the specified directory.
         * 
         * @param dir The directory to search for libraries.
         * @param libs A reference to a string_list that will be populated with the names of the libraries found.
         * @return A boolean indicating whether the operation was successful.
         */
        bool get_libs(const string& dir, string_list& libs) const;

        /**
         * @brief
         * Recursively get the youngest write time of header files in the given 
         * directory.
         * 
         * @param dir_path The path of the directory to search for header files.
         * @return The youngest write time of header files in the directory, or an 
         *         empty file_ts if the directory does not exist or an error occurs.
         */
        file_ts get_headers_write_time(const string& dir_path) const;
        
        /**
         * @brief
         * Lists all projects in the projects directory.
         * 
         * @return A string containing the names of all projects, each on a new line.
         */
        string list_projects() const;

        /**
         * @brief
         * Lists all modules in the modules directory.
         * 
         * @return A string containing the names of all modules, each on a new line.
         */
        string list_modules() const;    

        bool clear_project(const string& project_name, bool debug);
        bool clear_active_project(bool debug);
    private:
        env_vars vars;        

        static bool singleton_flag;
    };
}

extern ltd::ltd_home the_home;

#endif // _LTD_HOME_HPP_