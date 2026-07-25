#include "../inc/ltd/log.hpp"
#include "../inc/ltd/log_media_console.hpp"

#include "../inc/ltd/fmt.hpp"
#include "../inc/ltd/cli.hpp"

#include <thread>

namespace ltd
{
    log_writer log::writer;

    LOG_LEVEL log::log_level = LOG_LEVEL::INFO;

    void log::set_filter_level(LOG_LEVEL level)
    {
        log_level = level;
    }

    LOG_LEVEL log::get_filter_level()
    {
        return log_level;
    } 

    void log::set_verbosity(int verbosity)
    {
        log_level = (LOG_LEVEL) ((int)LOG_LEVEL::INFO + verbosity);
    }

    void log::init_console_logger()
    {
        // Create console log media
        auto console_media = new log_media_console();
        console_media->set_use_colors(true);

        // Add to writer medias
        writer.add_media(console_media);
    }
} // namespace ltd