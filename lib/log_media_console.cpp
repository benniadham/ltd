#include "../inc/ltd/cli.hpp"
#include "../inc/ltd/log_media_console.hpp"

namespace ltd
{
    const char* log_media_console::log_level_colors[] = 
    {
        "\033[1;31m", "\033[0;31m", "\033[0;33m", "\033[0,97m", "\033[93m", "\033[92m"
    };

    const char* log_media_console::log_color_reset = "\033[0m";

    void log_media_console::println(LOG_LEVEL level, const std::string& message)
    {
        if(!use_colors) 
            cli::println(message);        
        else
            cli::println("%s%s%s", log_level_colors[(int)level], message, log_color_reset);
    }

    void log_media_console::flush() 
    {
        std::cout.flush();
    }
}