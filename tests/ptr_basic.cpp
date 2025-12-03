#include "../inc/ltd/test_unit.hpp"
#include "../inc/ltd/cli.hpp"
#include "../inc/ltd/stdltd.hpp"
#include "../inc/ltd/allocators.hpp"

#include <vector>

using namespace ltd;

struct example
{
    static uint64_t instance_count;

    example()
    {
        instance_count++;
    }
    
    uint64_t count() const noexcept {
        return instance_count;
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
            tu.expect(ex1->count(), 1);
        }

        tu.expect(example::instance_count, 0);
    });
    
    tu.test([&tu](){
        {
            example *e = new example();
            auto obj = attach_object(e);
            tu.expect(obj->count(), 1);
        }
        tu.expect(example::instance_count, 0);
    });

    tu.test([&tu](){
        {
            pointer<example> ptr1;            
            {                    
                auto [ex1, err] = create_object<example>(system_memory_pool::get_instance());
                tu.expect(example::instance_count, 1);

                ptr1 = ex1.get_reference();
                tu.expect(ptr1.is_valid(), true);

                {
                    pointer<example> ptr2;
                    ptr2 = ptr1;
                    
                    tu.expect(ptr2.is_valid(), true);
                    tu.expect(ptr2->count(), 1);
                    tu.expect(ex1.get_reference_count(), 3);
                }

                tu.expect(ex1->count(), 1);
            }

            tu.expect(ptr1.is_valid(), false);
            tu.expect(example::instance_count, 1);
        }
        tu.expect(example::instance_count, 0);
    });

    tu.test([&tu](){

        std::vector<object<example>> samples;
        
        {
            for(int i=0; i<10; i++) {
                auto [ex1, err] = create_object<example>(system_memory_pool::get_instance());
                if (err == err::no_error) {
                    samples.push_back(std::move(ex1));
                }
            }
            
            tu.expect(example::instance_count, 10);
        }

        samples.clear();
        tu.expect(example::instance_count, 0);
    });

    tu.run(argc, argv);
    
    return 0;
}