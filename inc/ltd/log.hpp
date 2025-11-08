#ifndef _LTD_INCLUDE_LOG_HPP_
#define _LTD_INCLUDE_LOG_HPP_

#include "stddef.hpp"
#include "fmt.hpp"

#include "log_level.hpp"
#include "log_writer.hpp"

namespace ltd
{
    /**
     * @brief
     * Provides logging functionalities.
     */ 
    class log
    {
    public:
        static log_writer writer;
        
    private:
        static LOG_LEVEL log_level;

    public:
        static void set_filter_level(LOG_LEVEL level);
        static LOG_LEVEL get_filter_level();

        static void set_verbosity(int verbosity);       // Set log level using verbosity level 0-4

        static void init_console_logger();              // Initialize console logger

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

        /**
         * @brief
         * Print in verbosity level 'error'
         */
        template<typename T>
        static void fatal(T arg)
        {
            vprintln(LOG_LEVEL::FATAL, arg);
        }

        /**
         * @brief
         * Print in verbosity level 'error'
         */
        template<typename... Args>
        static void fatal(const char* format, Args... args)
        {
            vprintln(LOG_LEVEL::FATAL, format, args...);
        }

        /**
         * @brief
         * Print in verbosity level 'error'
         */
        template<typename T>
        static void error(T arg)
        {
            vprintln(LOG_LEVEL::ERROR, arg);
        }

        /**
         * @brief
         * Print in verbosity level 'error'
         */
        template<typename... Args>
        static void error(const char* format, Args... args)
        {
            vprintln(LOG_LEVEL::ERROR, format, args...);
        }

        /**
         * @brief
         * Print in verbosity level 'info'
         */
        template<typename T>
        static void info(T arg)
        {
            vprintln(LOG_LEVEL::INFO, arg);
        }

        /**
         * @brief
         * Print in verbosity level 'info'
         */
        template<typename... Args>
        static void info(const char* format, Args... args)
        {
            vprintln(LOG_LEVEL::INFO, format, args...);
        }

        /**
         * @brief
         * Print in verbosity level 'warn'
         */
        template<typename T>
        static void warn(T arg)
        {
            vprintln(LOG_LEVEL::WARN, arg);
        }

        /**
         * @brief
         * Print in verbosity level 'warn'
         */
        template<typename... Args>
        static void warn(const char* format, Args... args)
        {
            vprintln(LOG_LEVEL::WARN, format, args...);
        }

        /**
         * @brief
         * Print in verbosity level 'debug'
         */
        template<typename T>
        static void debug(T arg)
        {
            vprintln(LOG_LEVEL::DEBUG, arg);
        }

        /**
         * @brief
         * Print in verbosity level 'debug'
         */
        template<typename... Args>
        static void debug(const char* format, Args... args)
        {
            vprintln(LOG_LEVEL::DEBUG, format, args...);
        }

        /**
         * @brief
         * Print in verbosity level 'trace'
         */
        template<typename T>
        static void trace(T arg)
        {
            vprintln(LOG_LEVEL::TRACE, arg);
        }

        /**
         * @brief
         * Print in verbosity level 'trace'
         */
        template<typename... Args>
        static void trace(const char* format, Args... args)
        {
            vprintln(LOG_LEVEL::TRACE, format, args...);
        }
    };
} // namespace ltd

#endif // _LTD_INCLUDE_LOG_HPP_