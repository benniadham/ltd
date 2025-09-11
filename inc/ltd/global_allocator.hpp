#ifndef _LTD_INCLUDE_GLOBAL_ALLOCATOR_HPP_
#define _LTD_INCLUDE_GLOBAL_ALLOCATOR_HPP_

#include <vector>

#include "memory.hpp"
#include "stddef.hpp"

namespace ltd
{
    using global_allocator_type = std::conditional<is_defined<global_allocator>, global_allocator, null_allocator>::type;

    extern global_allocator_type global_allocator_instance;
} // namespace ltd

#endif // _LTD_INCLUDE_GLOBAL_ALLOCATOR_HPP_