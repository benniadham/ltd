#ifndef _LTD_INCLUDE_ALLOCATOR_HPP_
#define _LTD_INCLUDE_ALLOCATOR_HPP_

#include <vector>
#include "stdltd.hpp"

namespace ltd
{
    /**
     * @brief
     * System memory pool using malloc/free
     */
    struct system_memory_pool : memory_pool
    {
        static system_memory_pool *get_instance() noexcept;

        multi_ret<void*,memory_pool*,err> allocate(size_t allocation_size) noexcept override;

        err deallocate(void  *allocated_ptr) noexcept override;
    };
    
    /**
     * @brief
     * Global allocator. Orchestrates multiple memory pools.
     */
    struct global_allocator : public memory_pool
    {
        static global_allocator *get_instance();

        multi_ret<void*,memory_pool*,err> allocate(size_t allocation_size) noexcept override;

        err deallocate(void  *allocated_ptr) noexcept override;

    private:
        std::vector<memory_pool*> local_pools;
    };


}  // namespce ltd

#endif // _LTD_INCLUDE_ALLOCATOR_HPP_