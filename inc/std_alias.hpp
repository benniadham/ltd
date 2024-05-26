#ifndef _LTD_INCLUDE_STD_ALIAS_HPP_
#define _LTD_INCLUDE_STD_ALIAS_HPP_

#include <tuple>
#include <typeinfo>

namespace ltd
{
    /**
     * @brief
     * Tuple definition for multiple return values support.
     * 
     * This type is used to return multiple values from a function. Use the 
     * struct initialization format To return multiple values from a function.
     */
    template<typename... Args>
    using multi_ret = std::tuple<Args...>;

     /**
     * @brief
     * Catches values using declared variable from function returns. 
     */
    template<typename... Args>
    auto catch_ret(Args&... args)
    {
        return std::tuple<Args&...>(args...);
    }
    
} // namespace ltd

#endif // _LTD_INCLUDE_STD_ALIAS_HPP_