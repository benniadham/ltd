#ifndef _LTD_INCLUDE_STD_ALIAS_HPP_
#define _LTD_INCLUDE_STD_ALIAS_HPP_

#include <cstddef>
#include <tuple>
#include <typeinfo>
#include <string>
#include <vector>

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
    
    using string = std::string;
    using string_list = std::vector<string>;

    string_list split(const string& text, const string& delimiter);

} // namespace ltd

#endif // _LTD_INCLUDE_STD_ALIAS_HPP_