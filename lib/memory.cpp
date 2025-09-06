#include "../inc/ltd/memory.hpp"

using namespace ltd;

multi_ret<block,err> null_allocator::allocate(size_t allocation_size)
{
    return {{nullptr, 0}, err::allocation_failure};
}

err null_allocator::deallocate(block allocated_block)
{
    return err::deallocation_failure;
}

multi_ret<bool,err> null_allocator::owns(block mem_block)
{
    return {false, err::no_error};
}

multi_ret<block,err> global_allocator_stub::allocate(size_t allocation_size)
{
    return {{nullptr, 0}, err::allocation_failure};
}

err global_allocator_stub::deallocate(block allocated_block)
{
    return err::deallocation_failure;
}

multi_ret<block,err> local_allocator_stub::allocate(size_t allocation_size)
{
    return {{nullptr, 0}, err::allocation_failure};
}

err local_allocator_stub::deallocate(block allocated_block)
{
    return err::deallocation_failure;
}