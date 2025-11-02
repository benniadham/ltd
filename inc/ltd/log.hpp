#ifndef _LTD_INCLUDE_LOG_HPP_
#define _LTD_INCLUDE_LOG_HPP_

#include "stddef.hpp"
#include "fmt.hpp"

namespace ltd
{
    /**
     * @brief
     * Provides logging functionalities.
     * 
     * Log levels:
     * Trace  - Userd by developer for code execution tracing.
     * Debug  - Information that is diagnostically helpful to sysadmins.
     * Info   - Generally useful information to log (service start/stop, configuration assumptions, etc). 
     *          Info I want to always have available but usually don't care about under normal circumstances. This is my out-of-the-box config level.
     * Warn   - Anything that can potentially cause application oddities, but for which I am automatically recovering. (Such as switching from a primary to backup server, retrying an operation, missing secondary data, etc.)
     * Error  - Any error which is fatal to the operation, but not the service or application (can't open a required file, missing data, etc.). These errors will force user (administrator, or direct user) intervention. These are usually reserved (in my apps) for incorrect connection strings, missing services, etc.
     * Fatal  - Any error that is forcing a shutdown of the service or application to prevent data loss (or further data loss). I reserve these only for the most heinous errors and situations where there is guaranteed to have been data corruption or loss.
     */
    enum class LOG_LEVEL
    {
        FATAL,
        ERROR,
        WARN,
        INFO,
        DEBUG,
        TRACE
    };

    /**
     * @brief
     * Log media interface.
     */
    class log_media
    {
    public:
        virtual void println(LOG_LEVEL level, const std::string& message) = 0;
        virtual void flush() = 0;
    };

    /**
     * @brief
     * Console log media implementation.
     */
    class console_log_media : public log_media
    {
    public:
        void println(LOG_LEVEL level, const std::string& message) override;
        void flush() override;
    };

    /**
     * @brief
     * Colored log media implementation.
     */
    class colored_log_media : public log_media
    {
    public:
        void println(LOG_LEVEL level, const std::string& message) override;
        void flush() override;

    private:
        static const char* log_level_colors[];
        static const char* log_color_reset;
    };

    /**
     * @brief
     * Log writing functionalities.
     */
    class log_writer
    {
    public:
        log_writer();
        void write(LOG_LEVEL level, const std::string& message);
        void set_time_format(const string& format); 

    private:
        std::vector<log_media*> medias;

        string time_format = "%Y-%m-%d %H:%M:%S";

        bool print_timestamps = false;
        bool print_thread_ids = false;
        bool print_level_prefix = false;

        static const char* log_level_strings[];
    };

    /**
     * @brief
     * Provides logging functionalities.
     */ 
    class log
    {
    public:
        
    private:
        static LOG_LEVEL log_level;

        static log_writer writer;

    public:
        static void set_filter_level(LOG_LEVEL level);
        static LOG_LEVEL get_filter_level();

        /**
         * @brief
         * Print line using verbosity level.
         */
        template<typename T>
        static void vprintln(LOG_LEVEL level, T arg)
        {
            if(level <= log_level) {
                auto formatted_message = fmt::sprintf(arg);
                writer.write(level, formatted_message);
            }
        }

        /**
         * @brief
         * Print line using verbosity level.
         */
        template<typename... Args>
        static void vprintln(LOG_LEVEL level, const char* format, Args... args)
        {
            if(level <= log_level) {
                auto formatted_message = fmt::sprintf(format, args...);
                writer.write(level, formatted_message);
            }                
        }
    };
} // namespace ltd

#endif // _LTD_INCLUDE_LOG_HPP_