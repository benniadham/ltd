#ifndef _LTD_INCLUDE_ALLOCATOR_HPP_
#define _LTD_INCLUDE_ALLOCATOR_HPP_

#include "stddef.hpp"
#include "err.hpp"

namespace ltd
{
    /**
     * @brief 
     * Check whether a type already defined or just declared. 
     */
    template<typename T, typename = void>
    constexpr bool is_defined = false;

    /**
     * @brief 
     * Check whether a type already defined or just declared. 
     */
    template<typename T>
    constexpr bool is_defined<T, decltype(typeid(T), void())> = true;

    /**
     * @brief
     * This namespace provides functionalities for memory allocators. The framework
     * partially inspired by Andrei Alexandrescu talks regarding composables allocators.
     * 
     * The default allocator is assigned to heap_allocator. To override this,
     * one should declare and implement class global_allocator.
     * 
     * To have a desired bespoke behaviour of an allocator, one can compose two or
     * more allocators. To use it globally, declare it as global_allocator or to
     * use it in a specific class, pass it as a template parameter.
     * 
     * The library uses this framework for dynamic object creation and internal allocation
     * procedures for containers.
     */
    namespace mem
    {
        /**
         * Declaration for global allocator 
         */
        class global_allocator;

        /**
         * block is used as the container for memory allocation and deallocation.
         */
        struct block
        {
            void  *ptr;
            size_t size;
        };

        /**
         * The standard interface for allocators.
         */
        class null_allocator
        {
        public:
            multi_ret<block,err> allocate(size_t allocation_size);
            multi_ret<block,err> allocate_all();

            err deallocate(block allocated_block);
            err deallocate_all();

            err expand(block& allocated_block, size_t delta);

            multi_ret<bool,err> owns(block mem_block);
        };
    }  // namespace mem
}  // namespce ltd

#endif // _LTD_INCLUDE_ALLOCATOR_HPP_