#ifndef _LTD_COMMAND_HANDLERS_HPP_
#define _LTD_COMMAND_HANDLERS_HPP_

#include "commands.hpp"
#include "ltd_home.hpp"

namespace ltd
{
    /**
     * @brief 
     * Show the configuration with the given name
     * 
     * @param config_name The name of the configuration to show
     */
    void show_config(const string& config_name);

    /**
     * @brief 
     * Show all projects
     */
    bool ls();
}

#endif // _LTD_COMMAND_HANDLERS_HPP_