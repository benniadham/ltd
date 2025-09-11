#ifndef _LTD_INCLUDE_SDK_HPP_
#define _LTD_INCLUDE_SDK_HPP_

#include <filesystem>
#include "../inc/ltd/stddef.hpp"

namespace fs = std::filesystem;

namespace ltd
{
    namespace sdk
    {
        enum Cmd
        {
            CMD_INVALID,
            CMD_LS,
            CMD_PWD,
            CMD_CD,
            CMD_BUILD,
            CMD_CLEAN,
            CMD_TEST,
            CMD_DEPLOY,
            CMD_HELP, 
            CMD_GET
        };

        /**
         * @brief
         * Check whether the LTD_HOME environment varianle is set.
         */
        bool is_homepath_set();

        /**
         * @brief
         * Get the home path for LTD.
         */
        string get_homepath();

        /**
         * @brief 
         * Read the currently active project from config file.
         */
        string get_active_project();

        /**
         * @brief
         * Write the active project and store it in config file.
         */
        void set_active_project(const string& project);

        /**
         * @brief
         * Get the active project absolute path.
         */
        string get_active_project_path();

        /**
         * @brief
         * Get the builds directory under home path.
         */
        string get_builds_path();

        /**
         * @brief
         * Get the active build absolute path.
         */
        string get_active_build_path(bool debug);

        /**
         * @brief
         * Copy the project files into importable modules path.
         */
        void deploy_to_module_path();

        /**
         * @brief
         * Get a list of available LTD projects
         */
        void get_projects_list(string_list& projects);

        /**
         * @brief
         * Get a list of available LTD modules
         */
        void get_modules_list(string_list& modules);

        /**
         * @brief
         * Iterate source folder under current active projects
         */
        void list_project_dir(string_list& dirs);

        /**
         * @brief
         * Build a source dir into a target binary, executable or static library.
         */
        void build_dir(const string& name, const string& sub_dir, bool debug, string_list& imports);

        /**
         * @brief
         * Get directory write timestamp.
         */
        fs::file_time_type get_dir_write_time(const string& path);

        /**
         * @brief
         * Convert file_time_type to human readable string.
         */
        string file_time_to_string(const fs::file_time_type& timestamp);

        /**
         * @brief
         * Clean all binaries from the build directory.
         */
        void clean_project(bool debug);
    }
}

#endif // _LTD_INCLUDE_SDK_HPP_

