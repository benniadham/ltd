#ifndef _LTD_INCLUDE_LOG_WRITER_HPP_
#define _LTD_INCLUDE_LOG_WRITER_HPP_

#include <vector>

#include "stddef.hpp"
#include "log_level.hpp"
#include "log_media.hpp"

namespace ltd
{
    /**
     * @brief
     * Log writing functionalities.
     */
    class log_writer
    {
    public:
        log_writer();
        void write(LOG_LEVEL level, const string& message);
        void set_time_format(const string& format); 

        void enable_timestamps(bool enable) { print_timestamps = enable; }
        void enable_thread_ids(bool enable) { print_thread_ids = enable; }
        void enable_level_prefix(bool enable) { print_level_prefix = enable; }

        void add_media(log_media* media) { medias.push_back(media); }
        
    private:
        std::vector<log_media*> medias;

        string time_format = "%Y-%m-%d %H:%M:%S";

        bool print_timestamps = false;
        bool print_thread_ids = false;
        bool print_level_prefix = false;

        static const char* log_level_strings[];
    };
}

#endif // _LTD_INCLUDE_LOG_WRITER_HPP_