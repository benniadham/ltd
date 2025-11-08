#ifndef _LTD_INCLUDE_LOG_MEDIA_HPP_
#define _LTD_INCLUDE_LOG_MEDIA_HPP_

#include "stddef.hpp"
#include "log_level.hpp"

namespace ltd
{
    /**
     * @brief
     * Log media interface.
     */
    class log_media
    {
    public:
        virtual void println(LOG_LEVEL level, const string& message) = 0;
        virtual void flush() = 0;
    };
}
#endif // _LTD_INCLUDE_LOG_MEDIA_HPP_