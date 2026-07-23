#ifndef _LTD_BUILD_PLAN_HPP_
#define _LTD_BUILD_PLAN_HPP_

#include <vector>

#include "../inc/ltd/stdltd.hpp"

#include "build_job.hpp"

namespace ltd
{
    class build_plan
    {
    private:
        std::vector<object<build_job>> jobs;

    public:
       /**
        * @brief
        * Creates a new build plan based on the provided parameters
        */ 
       build_plan() {}

       /**
        * @brief
        * Creates a new build plan based on the provided parameters. 
        * 
        * @param params The build parameters for the plan.
        * @param home The ltd_home object representing the home directory.
        * 
        * @return True if the build plan was created successfully, false otherwise.
        */
       bool create(build_parameters& params, const ltd_home& home);

       /**
        * @brief
        * Executes all jobs in this build plan.  
        * 
        * @return True if all jobs were executed successfully, false otherwise.
        */
       bool execute();
    };
} // namespace ltd

#endif // _LTD_BUILD_PLAN_HPP_