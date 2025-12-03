#include "../inc/ltd/test_unit.hpp"
#include "../inc/ltd/cli.hpp"
#include "../inc/ltd/stdltd.hpp"
#include "../inc/ltd/allocators.hpp"

using namespace ltd;

struct example
{
    static uint64_t instance_count;

    example()
    {
        instance_count++;
    }
    
    ~example()
    {
        instance_count--;
    }
};

uint64_t example::instance_count = 0;

auto main(int argc, char** argv) -> int
{
    test_unit tu;
   
    tu.test([&tu](){
        {
            auto [ex1, err] = create_object<example>(system_memory_pool::get_instance());
            tu.expect(example::instance_count, 1);
        }

        tu.expect(example::instance_count, 0);
    });
    
    tu.test([&tu](){
        {
            example *e = new example();
            auto obj = attach_object(e);
            tu.expect(example::instance_count, 1);
        }

        tu.expect(example::instance_count, 0);
    });

    tu.run(argc, argv);
    
    return 0;
}