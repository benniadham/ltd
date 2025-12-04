#include "../inc/ltd/allocators.hpp"

namespace ltd
{
    system_memory_pool* system_memory_pool::get_instance() noexcept
    {
        static system_memory_pool instance;
        return &instance;
    }   

    multi_ret<void*,memory_pool*,err> system_memory_pool::allocate(size_t allocation_size) noexcept
    {
        void *ptr = std::malloc(allocation_size);

        if (ptr == nullptr) {
            return {nullptr, this, err::allocation_failure};
        }

        return {ptr, this, err::no_error};
    }

    err system_memory_pool::deallocate(void  *allocated_ptr) noexcept
    {
        std::free(allocated_ptr);
        return err::no_error;
    }

    global_allocator *global_allocator::get_instance()
    {
        static global_allocator instance;
        return &instance;
    }
 
    // Global allocator methods
    // Temporary simple implementation using new/delete
    multi_ret<void*,memory_pool*,err> global_allocator::allocate(size_t allocation_size) noexcept
    {
        void *ptr = ::operator new(allocation_size, std::nothrow);

        if (ptr == nullptr) {
            return {nullptr, this, err::allocation_failure};
        }

        return {ptr, this, err::no_error};
    }

    // Global allocator methods
    // Temporary simple implementation using new/delete
    err global_allocator::deallocate(void  *allocated_ptr) noexcept
    {
        ::operator delete(allocated_ptr);
        return err::no_error;
    }

}  // namespce ltd