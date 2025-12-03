#ifndef _LTD_INCLUDE_ALLOCATOR_HPP_
#define _LTD_INCLUDE_ALLOCATOR_HPP_

#include <vector>
#include "stdltd.hpp"

namespace ltd
{
    struct ref_counter_pool 
    {
        static ref_counter_pool *get_instance() noexcept;
        multi_ret<void*,err> allocate() noexcept;
        err deallocate(void* allocated_rc) noexcept;
    };

    struct system_memory_pool : memory_pool
    {
        static system_memory_pool *get_instance() noexcept;

        multi_ret<void*,memory_pool*,err> allocate(size_t allocation_size) noexcept override;

        err deallocate(void  *allocated_ptr) noexcept override;
    };
    
    struct global_allocator : public memory_pool
    {
        static global_allocator *get_instance()
        {
            static global_allocator instance;
            return &instance;
        }

        multi_ret<void*,memory_pool*,err> allocate(size_t allocation_size) noexcept override;

        err deallocate(void  *allocated_ptr) noexcept override;

    private:
        std::vector<memory_pool*> local_pools;
    };


}  // namespce ltd

#endif // _LTD_INCLUDE_ALLOCATOR_HPP_