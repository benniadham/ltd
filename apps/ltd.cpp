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
    std::string src;
    std::string bin;
    std::string tmp;
    std::string prj;

    std::string get_project_path() const {
        return src + prj + "/";
    }

} paths;

struct {

} timestamps;

void print_help();
void resolve_home();
void check_home_dirs();
void ensure_path(const std::string& path);
void check_prj_dirs(const std::string& project);
void build_library();
void prepare_flags();

auto main(int argc, char** argv) -> int 
{
    ltd::cli_flags flags;

    int verbosity=0;

    flags.bind(&verbosity, 'v', "verbose", "Set verbosity level.");
    flags.parse(argc, argv);

    std::string cmd;
    std::string prj;
    ltd::status ret;

    if (flags.size() < 2) {
        print_help();
        flags.print_help(4);
        exit(1);
    }

    catch_ret(cmd, ret) = flags.at(1);
        
    if (cmd == "help") {
        print_help();
        flags.print_help(4);
        return 0;
    }

    std::cout << "Resolving home directory...\n";
    resolve_home();
    std::cout << "\tHome is '" + paths.home + "'" << std::endl;

    std::cout << "Validating home directory...";
    check_home_dirs();
    std::cout << " done" << std::endl;

    // Build, clean and other commands
    if (flags.size() < 3) {
        std::cout << "Not enough arguments... quiting..." << std::endl;
        print_help();
        flags.print_help(4);
        exit(1);
    }

    catch_ret(prj, ret) = flags.at(2);
    paths.prj = prj;

    if (cmd == "build") 
    {
        std::cout << "Building '" << prj << "'..." << std::endl;
        if(fs::exists(paths.src + prj) == false) {
            std::cout << "Unable to find project '" << prj << "'. Quiting..." << std::endl;
            exit(1);
        }

        build_library();
        return 0;
    }

    if (cmd == "clean" )
    {
        return 0;
    }
    
    return 0;
}

void check_prj_dirs(const std::string& prj)
{
    // check src
    

    // check tmp

    // check bin
}

void check_home_dirs()
{
    // Home directory has to have 3 folders:
    // - src for project source files
    // - bin for binary products
    // - tmp for temporary compiled objects

    paths.src = paths.home + "src/";
    paths.bin = paths.home + "bin/";
    paths.tmp = paths.home + "tmp/"; 

    ensure_path(paths.src);
    ensure_path(paths.bin);
    ensure_path(paths.tmp);
}

void build_library()
{
    //if (fs::exists(paths.src + prj + "/lib")) {
    //    std::cout << "Project 'ltd' lib building is enabled."
    //}
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

    if (fs::exists(paths.home) == false) {
        std::cout << "Cannot resolve home.. exiting\n";
        exit(1);
    }

    std::cout << "Home is: " << paths.home << std::endl;
}

void prepare_flags()
{

}

void ensure_path(const std::string& path)
{
    if (fs::exists(path) == false) {
        fs::create_directory(path);
    }
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
