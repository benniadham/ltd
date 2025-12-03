#include "../inc/ltd/test_unit.hpp"
#include "../inc/ltd/fmt.hpp"
#include "../inc/ltd/cli.hpp"
#include "../inc/ltd/stdltd.hpp"

#include <stacktrace>

#include <variant>
#include <memory>

using namespace ltd;

class d
{
    //int *a;
};

void stack_trace_example()
{
    std::stacktrace st = std::stacktrace::current();

    for(auto ste : st) {
        cli::println("Frame: %d, %s", ste.source_line(), ste.description().c_str());
    }
}

auto main(int argc, char** argv) -> int
{
    test_unit tu;

    tu.test([&tu](){
        std::string result = fmt::sprintf("'%.2f'", 1.23);
        tu.expect(result, "'1.23'");
    });
    
    tu.run(argc, argv);
    
    std::unique_ptr<d> a;
    std::shared_ptr<d> b;
    object<d> c;

    // cli::println("Size of Unique %d", sizeof( std::unique_ptr<d>));
    // cli::println("Size of Shared %d", sizeof( std::shared_ptr<d>));
    // cli::println("Size of Variant %d", sizeof( std::variant<std::unique_ptr<d>,std::shared_ptr<d>>));
    // cli::println("Size of object<d> %d", sizeof( object<d>));
    // cli::println("Size of Deleter %d", sizeof( default_dltr<d> ));
    // cli::println("Size of D %d", sizeof( d ));

    // stack_trace_example();

    return 0;
}