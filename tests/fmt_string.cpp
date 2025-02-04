#include "../inc/ltd/test_unit.hpp"
#include "../inc/ltd/fmt.hpp"

using namespace ltd;

auto main(int argc, char** argv) -> int
{
    test_unit tu;

    tu.test([&tu](){
        std::string result = fmt::sprintf("'%s'", "string");
        tu.expect(result, "'string'");
    });

    tu.test([&tu](){
        std::string result = fmt::sprintf("'%10s'", "string");
        tu.expect(result, "'    string'");
    });

    tu.test([&tu](){
        std::string result = fmt::sprintf("'%-10s'", "string");
        tu.expect(result, "'string    '");
    });
    
    tu.run(argc, argv);
    
    return 0;
}