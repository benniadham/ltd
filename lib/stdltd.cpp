#include "../inc/ltd/stdltd.hpp"

namespace ltd
{
    ref_counter* rc_factory::create_ref_counter(memory_pool *pool) noexcept
    {
        return new ref_counter(pool);
    }

    void rc_factory::destroy_ref_counter(ref_counter *rc) noexcept
    {
        delete rc;
    }
} // namespace ltd