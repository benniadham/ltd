#ifndef _LTD_INCLUDE_SDK_HPP_
#define _LTD_INCLUDE_SDK_HPP_

#include <filesystem>

namespace fs = std::filesystem;

#include "../inc/flags.hpp"

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
            CMD_CLEAN
        };

        /**
         * @brief
         * Parse the cli arguments.
         */
        void parse_flags(Flags& flags);

        /**
         * @brief
         * Check whether the LTD_HOME environment varianle is set.
         */
        bool is_homepath_set();

        /**
         * @brief
         * Get the home path for LTD.
         */
        String get_homepath();

        /**
         * @brief 
         * Read the currently active project from config file.
         */
        String get_active_project();

        /**
         * @brief
         * Write the active project and store it in config file.
         */
        void set_active_project(const String& project);

        /**
         * @brief
         * Get the active project absolute path.
         */
        String get_active_project_path();

        /**
         * @brief
         * Get the builds directory under home path.
         */
        String get_builds_path();

        /**
         * @brief
         * Get the active build absolute path.
         */
        String get_active_build_path(bool debug);

        /**
         * @brief
         * Get a list of available LTD projects
         */
        void get_projects_list(StringList& projects);

        /**
         * @brief
         * Iterate source folder under current active projects
         */
        void list_project_dir(StringList& dirs);

        /**
         * @brief
         * Build a source dir into a target binary, executable or static library.
         */
        void build_dir(const String& name, const String& sub_dir, bool debug);

        fs::file_time_type get_dir_write_time(const String& path);

        String file_time_to_string(const fs::file_time_type& timestamp);

        void clean_project(bool debug);
    }
}

#endif // _LTD_INCLUDE_SDK_HPP_

