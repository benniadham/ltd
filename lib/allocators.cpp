#include "../inc/ltd/allocators.hpp"

using namespace ltd;
using namespace ltd::mem;

multi_ret<block,err> null_allocator::allocate(size_t allocation_size)
{
    return {{nullptr, 0}, err::allocation_failure};
}

multi_ret<block,err> null_allocator::allocate_all()
{
    return {{nullptr, 0}, err::allocation_failure};
}

err null_allocator::deallocate(block allocated_block)
{
    return err::deallocation_failure;
}

err null_allocator::deallocate_all()
{
    return err::deallocation_failure;
}

err null_allocator::expand(block& allocated_block, size_t delta)
{
    return err::allocation_failure;
}

multi_ret<bool,err> null_allocator::owns(block mem_block)
{
    return {false, err::no_error};
}