#include "../inc/ltd/allocators.hpp"

namespace ltd
{
    ref_counter_pool* ref_counter_pool::get_instance() noexcept
    {
        static ref_counter_pool instance;
        return &instance;
    }

    multi_ret<void*,err> ref_counter_pool::allocate() noexcept
    {
        void *ptr = std::malloc(sizeof(ref_counter));

        if(ptr == nullptr) 
            return {nullptr, err::allocation_failure};

        return {ptr, err::no_error};
    }

    err ref_counter_pool::deallocate(void* allocated_rc) noexcept
    {
        std::free(allocated_rc);
        return err::no_error;
    }

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

    multi_ret<void*,memory_pool*,err> global_allocator::allocate(size_t allocation_size) noexcept
    {
        void *ptr = ::operator new(allocation_size, std::nothrow);

        if (ptr == nullptr) {
            return {nullptr, this, err::allocation_failure};
        }

        return {ptr, this, err::no_error};
    }

    err global_allocator::deallocate(void  *allocated_ptr) noexcept
    {
        ::operator delete(allocated_ptr);
        return err::no_error;
    }

}  // namespce ltd