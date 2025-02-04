#include "../inc/ltd/test_unit.hpp"
#include "../inc/ltd/fmt.hpp"

using namespace ltd;

auto main(int argc, char** argv) -> int
{
    test_unit tu;

    tu.test([&tu](){
        std::string result = fmt::sprintf("'%d'", 100);
        tu.expect(result, "'100'");
    });

    tu.test([&tu](){
        std::string result = fmt::sprintf("'%4d'", 100);
        tu.expect(result,"' 100'");
    });

    tu.test([&tu](){
        std::string result = fmt::sprintf("'%-4d'", 100);
        tu.expect(result,"'100 '");
    });

    tu.test([&tu](){
        std::string result = fmt::sprintf("'%d'", -100);
        tu.expect(result,"'-100'");
    });

    tu.test([&tu](){
        std::string result = fmt::sprintf("'%+d'", 100);
        tu.expect(result,"'+100'");
    });

    tu.test([&tu](){
        std::string result = fmt::sprintf("'%04d'", 100);
        tu.expect(result,"'0100'");
    });

    tu.test([&tu](){
        std::string result = fmt::sprintf("'%4.4d'", 100);
        tu.expect(result,"'0100'");
    });
    
    tu.run(argc, argv);
    
    return 0;
}