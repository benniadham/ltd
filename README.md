# {Ltd.}
A suite of C++ build tool and library.

{Ltd.}'s purpose is to provide environment to help writing safe and performant
C++ code. It does so by providing:
- a build tools that reduces the complexity of various C/C++ build tools and 
  toolchains 
- a library with a framework that encourages consistent and good practices in 
  writing safe and performant code.

## Building

```
> mkdir ltd_home
> cd ltd_home
> mkdir projects
> cd projects
> git clone https://github.com/benniadham/ltd.git
> cd ltd
> LTD_HOME=/path/to/your/ltd_home; export LTD_HOME
> source ./bootstrap.sh
```

## Testing
To use the test framework, one might use it as follow:

```C++
#include <ltd/test_unit.hpp>

using namespace ltd;

auto main(int argc, char** argv) -> int 
{
    test_unit tu;

    tu.test([&tu](){
        tu.expect(true, "Expected true");
    });

    tu.test([&tu](){
        tu.expect(true, "Expected true");
    });

    tu.test([&tu](){
        tu.expect(true, "Expected true");
    });
    
    tu.run(argc, argv);
    
    return 0;
}
```

Each test unit has several test cases. The number of the test case will be determined 
by calling the test binary. When the test binary called withoutarguments, it will 
return the number of tests available. To run the test, specify the test id in the cli 
argument.

```
>./mytest --id=1
```

Test id starts from 0. In this example, the program will run the second test case.

## Directory Structure

In this example 'myproject1' has multiple applications and multiple library. 'myproject2' only
has 1 application and 1 library. The names of the binaries are 'myproject2' and 'myproject2.a'
consecutively.

```
$LTD_HOME
  +- modules
  |    +- myproject1
  |         +- inc
  |
  +- builds
  |
  +- projects
      +- myproject1
      |    +- apps
      |    |  +- myapp1
      |    |  +- myapp2
      |    +- doc
      |    +- inc
      |    +- libs
      |    |  +- mylib1
      |    |  +- mylib2
      |    +- tests
      |
      +- myproject2
          +- app
          +- doc
          +- lib
          +- tests
```