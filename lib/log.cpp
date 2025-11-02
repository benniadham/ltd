#include "../inc/ltd/log.hpp"

#include "../inc/ltd/fmt.hpp"

#include <thread>

namespace ltd
{
    const char* log_writer::log_level_strings[] = {"FATAL", "ERROR", "WARN", "INFO", "DEBUG", "TRACE"};

    log_writer::log_writer() 
    {

    }

    void log_writer::write(LOG_LEVEL level, const string& message)
    {
        string formatted_message;
        
        if (print_timestamps)
        {
            auto t = std::time(nullptr);
            auto tm = *std::localtime(&t);
            char time_buffer[100];
            std::strftime(time_buffer, sizeof(time_buffer), time_format.c_str(), &tm);
            formatted_message += fmt::sprintf("[%s] ", time_buffer);
        }

        if (print_thread_ids)
        {
            auto thread_id = std::this_thread::get_id();
            formatted_message += fmt::sprintf("[Thread %zu] ", std::hash<std::thread::id>{}(thread_id));
        }

        if (print_level_prefix)
        {
            formatted_message += fmt::sprintf("[%s] ", log_level_strings[(int)level]);
        }

        for (auto media : medias)
        {
            media->println(level, formatted_message);
            media->flush();
        }
    }

    void log_writer::set_time_format(const string& format)
    {
        time_format = format;
    }

    LOG_LEVEL log::log_level = LOG_LEVEL::INFO;

    void log::set_filter_level(LOG_LEVEL level)
    {
        log_level = level;
    }

    LOG_LEVEL log::get_filter_level()
    {
        return log_level;
    }

    void console_log_media::println(LOG_LEVEL level, const std::string& message)
    {
        fmt::println(message);
    }

    void console_log_media::flush() 
    {
        std::cout.flush();
    }

    const char* colored_log_media::log_level_colors[] = {"\033[1;31m", "\033[0;31m", "\033[0;33m", "\033[0;32m", "\033[0;34m", "\033[0;37m"};
    const char* colored_log_media::log_color_reset = "\033[0m";

    void colored_log_media::println(LOG_LEVEL level, const std::string& message)
    {
        fmt::println("%s%s%s", log_level_colors[(int)level], message, log_color_reset);
    }

    void colored_log_media::flush() 
    {
        std::cout.flush();
    }
} // namespace ltd