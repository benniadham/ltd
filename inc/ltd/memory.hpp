#ifndef _LTD_INCLUDE_MEMORY_HPP_
#define _LTD_INCLUDE_MEMORY_HPP_

#include "stddef.hpp"
#include "err.hpp"

namespace ltd
{
    /**
     * Construct object of type T on the given memory address.
     */
    template<typename T, typename... Args>
    static void construct(T *instance, Args&&... args)
    {
        new (instance) T(std::forward<Args>(args)...);
    }

    /**
     * Destruct the object by calling its destructor.
     * This does not release the memory.
     */
    template<typename T>
    void destruct(T *instance)
    {
        (*instance).~T();
    }    

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
    class allocator
    {
    public:
        virtual multi_ret<block,err> allocate(size_t allocation_size) = 0;
        virtual err deallocate(block allocated_block) = 0;
        virtual multi_ret<bool,err> owns(block mem_block) = 0;
    };

    /**
     * Declaration for global allocator 
     */
    class global_allocator;

    class null_allocator
    {
    public:
        multi_ret<block,err> allocate(size_t allocation_size);
        err deallocate(block allocated_block);
        multi_ret<bool,err> owns(block mem_block);
    };

    /**
     * Static class to access global allocator
     */
    class global_allocator_stub
    {
    public:
        static multi_ret<block,err> allocate(size_t allocation_size);
        static err deallocate(block allocated_block);
    };

    /**
     * Static class to access local scopped allocator
     */
    class local_allocator_stub
    {
    public:
        static multi_ret<block,err> allocate(size_t allocation_size);
        static err deallocate(block allocated_block);
    };
} // namespace ltd

#endif // _LTD_INCLUDE_MEMORY_HPP_