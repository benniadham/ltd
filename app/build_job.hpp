#ifndef _LTD_BUILD_JOB_HPP_
#define _LTD_BUILD_JOB_HPP_

#include "../inc/ltd/stdltd.hpp"

#include "env.hpp"
#include "ltd_home.hpp"

namespace ltd
{
    struct build_parameters
    {
        string target;

        string_list imports;
        string_list inc_dirs;
        string_list lib_dirs;
        string_list libs;

        string cppstd;
        string cppflags;

        bool debug_mode;
    };

    enum class build_error
    {
        no_error,
        missing_target,
        invalid_target,
        build_failed
    };

    struct task
    {
        virtual build_error execute()=0;    
    };

    class compile_task : public task
    {
    public:
        compile_task(const string& source_file, const string& output_file, const build_parameters& params); 
    };

    class link_task : public task
    {
    public:        
        link_task(const string& target, const build_parameters& params);
    };

    class archive_task : public task
    {    
    public:
        archive_task(const string& target, const build_parameters& params);
    };

    class build_job
    {
    public:
        /**
         * Constructor for the build_job class. Initializes the build
         */
        build_job();

        ~build_job();

        /**
         * @brief
         * Initializes the build job by checking for the presence
         */
        bool initialize(const build_parameters& params, const ltd_home& home);
        
        bool plan();

        void run();

    private:
        std::vector<object<task>> tasks;
        string source_dir;
        string builds_dir;
        
        string target;
        string target_dir;
        
        bool is_lib;
    };
}

#endif // _LTD_BUILD_JOB_HPP_