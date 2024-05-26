#include <string>
#include <iostream>
#include <filesystem>

#include <pwd.h>
#include <unistd.h>

#include "../inc/cli_flags.hpp"

#define HOME_ENV "CPP_HOME"

namespace fs = std::filesystem;

struct {
    std::string home;
} paths;

struct {

} timestamps;

void print_help();
void resolve_home();
void build_library();
void prepare_flags();

auto main(int argc, char** argv) -> int 
{
    ltd::cli_flags flags;

    flags.parse(argc, argv);

    std::string cmd;
    ltd::status ret;

    catch_ret(cmd, ret) = flags.at(1);

    std::cout << cmd << std::endl;

    //build_library();

    return 0;
}

void build_library()
{
    resolve_home();

    if (fs::exists(paths.home) == false) {
        std::cout << "Cannot resolve home.. exiting\n";
        exit(1);
    }

    std::cout << "Home is: " << paths.home << std::endl;
}

void resolve_home()
{
    auto env_home = getenv(HOME_ENV); 

    if(env_home == NULL) 
    {
        struct passwd *pw = getpwuid(getuid());
        paths.home = pw->pw_dir;
        paths.home += "/cpp_home";
    } 
    else 
        paths.home = env_home;

    if (paths.home.back() != '/') {
        paths.home += "/";
    }
}

void prepare_flags()
{

}

void print_help() 
{
    std::cout << std::endl << std::endl;

    std::cout << "{Ltd.} is building tools for C/C++ source code. " << std::endl;
    std::cout << "Usage: ltd command [arguments]" << std::endl;
    std::cout << std::endl;
    std::cout << "Available commands:" << std::endl;
    std::cout << "  build       compile and build a project" << std::endl;
    std::cout << "  clean       clean a project from binaries and temporary objects" << std::endl;
    
    // std::cout << "  debug       compile and build a project for debugging" << std::endl;
    // std::cout << "  package     copy the whole binary, library and header files into one folder" << std::endl;
    // std::cout << "  doc         generate documentation for a project" << std::endl;
    // std::cout << "  get         get package for github" << std::endl;
    // std::cout << "  info        display information of the current environment" << std::endl;
    // std::cout << "  search      search package on github" << std::endl;
     
    std::cout << "  help        print this help" << std::endl;
    std::cout << "  test        run a test unit" << std::endl;
    std::cout << "              ltd test [project/testunit]" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Options:" << std::endl;
}
