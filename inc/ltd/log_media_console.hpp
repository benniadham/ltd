#ifndef _LTD_INCLUDE_CONSOLE_LOG_MEDIA_HPP_
#define _LTD_INCLUDE_CONSOLE_LOG_MEDIA_HPP_ 

#include "log_media.hpp"

namespace ltd
{
    /**
     * @brief
     * Console log media implementation.
     */
    class log_media_console : public log_media
    {
    public:
        void println(LOG_LEVEL level, const string& message) override;
        void flush() override;

        bool is_using_colors() const { return use_colors; }
        void set_use_colors(bool enable) { use_colors = enable; }
        
    private:
        bool use_colors = false;

    private:
        static const char* log_level_colors[];
        static const char* log_color_reset;
    };
} // namespace ltd
#endif // _LTD_INCLUDE_CONSOLE_LOG_MEDIA_HPP_