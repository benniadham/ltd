#ifndef _LTD_INCLUDE_OBJ_HPP_
#define _LTD_INCLUDE_OBJ_HPP_

#include "allocators.hpp"

namespace ltd
{
        /**
     * @brief
     * This the default object deleter for dynamic objects.
     * 
     * Deleters are used for dynamic objects to destory and deallocate its raw 
     * pointers. The default behaviour of this deleter depends on the type of
     * the raw pointer. If it is created by the framework, it will then use the 
     * the framework to destruct the object and then deallocate the memory. If 
     * it was created using C++'s `new`, it will then use C++'s `delete`. You 
     * can define your own deleter to use custom framework. 
     * 
     * Implement this struct to have a custom deleter and pass it to the object's
     * template parameter. This would be useful if you have a pointer returned
     * from external API such as `FILE*` where you can implement the deleter
     * to close the `FILE*` to prevent resource leak.
     * 
     * @tparam T The type of the pointer to delete.
     * @tparam A The allocator type.
     */
    template<class T, class A>
    struct default_dltr
    {
        err::type operator ()(T *ptr)
        {
            mem::destruct(ptr);

            mem::block blk;
            blk.size = sizeof(T);
            blk.ptr = ptr;

            A allocator;
            return allocator.deallocate(blk);
        }
    };
}
#endif // _LTD_INCLUDE_OBJ_HPP_