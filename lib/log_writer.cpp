#include "../inc/ltd/log_writer.hpp"

#include <ctime>
#include <thread>

#include "../inc/ltd/fmt.hpp"
#include "../inc/ltd/cli.hpp"

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

        formatted_message += message;

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
}