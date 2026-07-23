#include "build_job.hpp"

#include <filesystem>

namespace fs = std::filesystem;

#include "../inc/ltd/log.hpp"

namespace ltd
{
    build_job::build_job()
    {
        log::trace("build_job constructor called");
    }

    build_job::~build_job()
    {
        log::trace("build_job destructor called");
    }

    bool build_job::initialize(const build_parameters& params, const ltd_home& home)
    {
        log::info("Initializing build job for target '%s'", params.target);

        if (params.target.empty()) {
            log::error("Build job initialization failed: missing target");
            return false;
        }

        if (starts_with(params.target, "lib")) 
        {
            target = home.get_project_name();                    // Use project name as target for library 

            source_dir = home.get_active_project_dir() + "/lib";
            if(!fs::exists(source_dir) || !fs::is_directory(source_dir)) {
                log::error("Source directory does not exist or is not a directory: %s", source_dir);
                return false;
            }

            builds_dir = home.get_active_builds_dir(params.debug_mode) + "/lib";
            if(!fs::exists(builds_dir)) {
                fs::create_directories(builds_dir);
            }

            target_dir = home.get_active_builds_dir(params.debug_mode);

            is_lib = true;
        } 
        else if (starts_with(params.target, "libs/")) 
        {
            // Target library name
            target = params.target.substr(5); // Remove "libs/" prefix
            if(target.empty()) {
                log::error("Build job initialization failed: invalid target '%s'", params.target);
                return false;
            }

            // Target .o directory
            builds_dir = home.get_active_builds_dir(params.debug_mode) + params.target;
            if(!fs::exists(builds_dir)) {
                fs::create_directories(builds_dir);
            }
            
            // Target binary directory
            target_dir = home.get_active_builds_dir(params.debug_mode);

            source_dir = home.get_active_project_dir() + params.target;
            if(!fs::exists(source_dir)) {
                log::error("Build job initialization failed: source directory does not exist");
                return false;
            }

            is_lib = true;
        } 
        else if (starts_with(params.target, "apps/")) 
        {
            target = params.target.substr(5); // Remove "apps/" prefix
            if(target.empty()) {
                log::error("Build job initialization failed: invalid target '%s'", params.target);
                return false;
            }

            // Target .o directory
            builds_dir = home.get_active_builds_dir(params.debug_mode) + params.target;
            if(!fs::exists(builds_dir)) {
                fs::create_directories(builds_dir);
            }

            target_dir = home.get_active_builds_dir(params.debug_mode);

            source_dir = home.get_active_project_dir() + params.target;
            if (!fs::exists(source_dir)) {
                log::error("Build job initialization failed: source directory does not exist");
                return false;
            }
            
            is_lib = false;
        }
        else if (starts_with(params.target, "app")) 
        {
            target = home.get_project_name();   // Use project name as target for application

            source_dir = home.get_active_project_dir() + "app";
            if (!fs::exists(source_dir)) {
                log::error("Build job initialization failed: source directory does not exist");
            }

            builds_dir = home.get_active_builds_dir(params.debug_mode) + "/app";
            if(!fs::exists(builds_dir)) {
                fs::create_directories(builds_dir);
            }

            target_dir = home.get_active_builds_dir(params.debug_mode);

            is_lib = false;
        }
        else 
        {
            log::error("Build job initialization failed: invalid target '%s'", target);
            return false;
        }
        
        // Additional validation of parameters can be added here, such as checking 
        // for valid import paths, include directories, etc.
        
        log::info("Build job initialized successfully for target '%s'", params.target);
        
        log::trace("Build : source_dir='%s'", source_dir);
        log::trace("Build : builds_dir='%s'", builds_dir);
        log::trace("Build : target_dir='%s'", target_dir);
        log::trace("Build : target='%s'", target);

        return true;
    }

    bool build_job::plan()
    { 
        return true;
    }

    void build_job::run()
    {
        // log::info("Running build job for target '%s'", name_);
        // Implementation for running the build job would go here, such as invoking the compiler, linking, etc.
        // For now, we will just print a message indicating that the build job is running.
    }
}
