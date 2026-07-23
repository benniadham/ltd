#ifndef _LTD_BUILD_TOOLS_HPP_
#define _LTD_BUILD_TOOLS_HPP_

#include "../inc/ltd/stddef.hpp"
#include "../inc/ltd/stdfs.hpp"

#include "commands.hpp"
#include "ltd_home.hpp"

namespace ltd
{
    class build_tools
    {
    public:
        enum task_type {
            COMPILE,
            LINK,
            ARCHIVE
        };

        inline string task_type_to_string(task_type type) {
            switch(type) {
                case COMPILE:
                    return "compile";
                case LINK:
                    return "link";
                case ARCHIVE:
                    return "archive";
                default:
                    return "invalid task type";
            }
        }

        struct build_task
        {
            string      source;
            string      target;
            string      message;
            task_type   type;
        };

        using build_plan = std::vector<build_task>;

    public:
        build_tools();

        bool configure(const command_info& info);

        bool build_libs() const;
        bool build_lib(const string& sub_dir, const string& name) const;
        bool build_apps() const;
        bool build_app(const string& sub_dir, const string& name) const; 
        bool build_tests() const;
        
    private:        
        /**
         * @brief
         * Configure the include paths for the build tools by retrieving the builds directory 
         * and setting the include path accordingly.
         * 
         * @param imports A list of import names to include in the include path configuration.
         * @param includes A list of additional include directories to include in the include path configuration.
         * @return true if the include paths were successfully configured, false otherwise.
         */
        bool configure_inc_dir(const string_list& imports, const string_list& includes);

        /**
         * @brief 
         * Configure the library paths for the build tools by retrieving
         * the builds directory and setting the library path accordingly.
         * 
         * @param imports A list of import names to include in the library path configuration.
         * @param libraries A list of library names to include in the library path configuration.
         * @return true if the library paths were successfully configured, false otherwise.
         */
        bool configure_lib_dir(const string_list& imports, const string_list& libraries);

        /**
         * @brief
         * Execute the given build plan by performing each build task in sequence.
         */
        bool run_build_plan(build_plan& plan) const;

        /**
         * @brief
         * Compile all source files in the specified directory and add the resulting build tasks to the plan.
         */
        bool plan_dir_compilation(build_plan& plan, const string& sub_dir) const;

        /**
         * @brief 
         * Create a new library file from the object files in the given directory.
         */
        bool plan_lib_creation(build_plan& plan, const string& sub_dir, const string& name) const;

        /**
         * @brief
         * Link the object files in the specified directory to create an executable application.
         * 
         * @param plan The build plan to which the application linking tasks will be added.
         * @param sub_dir The subdirectory containing the object files to be linked.
         * @param name The name of the resulting executable application.
         * @return true if the application linking tasks were successfully added to the build plan, false
         */
        bool plan_app_linking(build_plan& plan, const string& sub_dir, const string& name) const;

        /**
         * @brief
         * Link the object files in the tests directory to create executable test suites.
         * 
         * @param plan The build plan to which the test linking tasks will be added.
         * @return true if the test linking tasks were successfully added to the build plan, false otherwise.
         */
        bool plan_tests_linking(build_plan& plan) const;

        /**
         * @brief
         * Determine if a build task needs to be recompiled based on the timestamps of the source and target files.
         * 
         * @param task The build task to check for recompilation.
         * @param dir_ts The timestamp of the directory containing the source file.
         * @return true if the build task needs to be recompiled, false otherwise.
         */
        bool need_recompile(const build_task& task, const file_ts& dir_ts) const;
        
        /**
         * @brief 
         * Compile a source file using the configured compiler and flags.
         * 
         * @param source The path to the source file to be compiled.
         * @param target The path to the output file to be generated.
         * @return true if the compilation was successful, false otherwise.
         */
        bool compile_file(const string& source, const string& target) const;

    private:
        file_ts lib_header_ts;

        string cpp = "g++";
        string cppstd;     
        string cppflags;
           
        string inc_dirs;
        string lib_dirs;
        string libs;

        bool debug;
    };
} // namespace ltd

#endif // _LTD_BUILD_TOOLS_HPP_