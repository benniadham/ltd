#include "build_plan.hpp"

#include "../inc/ltd/stdltd.hpp"
#include "../inc/ltd/allocators.hpp"
#include "../inc/ltd/log.hpp"

namespace ltd
{
    bool build_plan::create(build_parameters& params, const ltd_home& home)
    {
        if(params.target == "" || params.target == "all") 
        {
            auto dir = home.get_active_project_dir();
            log::trace("Iterate build targets under %s", dir);

            for(const auto& dir_entry : fs::directory_iterator(dir)) {
                if(dir_entry.is_directory()) {
                    log::trace("Found %s", dir_entry);

                    auto path = dir_entry.path().generic_string();
                    
                    if(ends_with(path, "/app")) 
                    {
                        params.target = "app";  
                        
                        build_job job;                        
                        if(job.initialize(params, home) == false) {
                            log::error("Failed to initialize build job");
                            return 1;   
                        }
                    } 
                    else if(ends_with(path, "/lib")) 
                    {
                        params.target = "lib";                            

                        auto [job, job_err] = create_object<build_job>(system_memory_pool::get_instance());              
                        if(job->initialize(params, home) == false) {
                            log::error("Failed to initialize build job");
                            return 1;   
                        }
                    } 
                    else if(path.find("/libs") != string::npos) 
                    {
                        for(auto const& subdir_entry : fs::directory_iterator(path)) {
                            log::trace("Found %s", subdir_entry);
                        }
                    } 
                    else if(path.find("apps") != string::npos) 
                    {
                        for(auto const& subdir_entry : fs::directory_iterator(path)) {
                            log::trace("Found %s", subdir_entry);
                        }
                    } 
                    else 
                    {
                        log::trace("Skipping %s", dir_entry);   
                        continue;
                    }                                            
                }
            }
        } 
        else if(params.target == "lib" || params.target == "app" || 
                starts_with(params.target, "apps/") || starts_with(params.target, "libs/"))
        {
            // build_job job;
            // if(job.initialize(params, home) == false) {
            //     log::error("Failed to initialize build job");
            //     return 1;   
            // }
        }
        else
        {
            log::error("Invalid target name: %s", params.target);
            return -1;
        }
        return true;
    }

    bool build_plan::execute()
    {
        return true;
    }
}