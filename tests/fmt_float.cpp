#include "../inc/ltd/test_unit.hpp"
#include "../inc/ltd/fmt.hpp"

using namespace ltd;

auto main(int argc, char** argv) -> int
{
    test_unit tu;

    tu.test([&tu](){
        std::string result = fmt::sprintf("'%.2f'", 1.23);
        tu.expect(result, "'1.23'");
    });

    tu.test([&tu](){
        std::string result = fmt::sprintf("'%4.1f'", 1.23);
        tu.expect(result,"' 1.2'");
    });

    tu.test([&tu](){
        std::string result = fmt::sprintf("'%-5.2f'", 1.23);
        tu.expect(result,"'1.23 '");
    });

    tu.test([&tu](){
        std::string result = fmt::sprintf("'%.2f'", -1.23);
        tu.expect(result,"'-1.23'");
    });

    tu.test([&tu](){
        std::string result = fmt::sprintf("'%+.2f'", 1.23);
        tu.expect(result,"'+1.23'");
    });

    tu.test([&tu](){
        std::string result = fmt::sprintf("'%05.2f'", 1.23);
        tu.expect(result,"'01.23'");
    });

    tu.test([&tu](){
        std::string result = fmt::sprintf("'%-6.4d'", 1.23);
        tu.expect(result,"'1.2300'");
    });
    
    tu.run(argc, argv);
    
    return 0;
}