#include "build_tools.hpp"

#include <cstdlib>

#include "../inc/ltd/log.hpp"

#include "ignore_filter.hpp"

namespace ltd
{
    build_tools::build_tools()
    {
        
    }

    bool build_tools::configure(const command_info& info)
    {
        lib_header_ts = the_home.read_lib_header_ts();

        debug = info.debug_mode > 0;

        // configure cpp flags
        cppflags = "-Wall";
        if (info.debug_mode)
            cppflags += " -g";
        else 
            cppflags += " -O3";
        cppflags += string(" -std=") + info.cppstd;        

        // configure include path
        if (configure_inc_dir(info.imports, info.inc_dirs) == false) {
            log::fatal("Failed configuring include paths");
            return false;
        }

        // configure libs and lib path
        if (configure_lib_dir(info.imports, info.libs, info.lib_dirs) == false) {
            log::fatal("Failed configuring lib paths");
            return false;
        }

        return true;
    }

    bool build_tools::configure_lib_dir(const string_list& imports, const string_list& libraries, const string_list& libdirs)
    {
        log::trace("Configuring library directories and libraries");

        // Add the active builds directory to the library search path
        auto project_build_dir = the_home.get_active_build_dir(debug);
        log::trace("Adding active build directory to library search path: %s", project_build_dir);
        lib_dirs = string("-L") + project_build_dir;

        // Add import library paths
        for (const string& import : imports) {
            auto module_dir = the_home.get_module_dir(import);
            log::trace("Adding import module directory to library search path: %s", module_dir);
            lib_dirs += string(" -L") + module_dir;

            string_list import_libs;
            if (the_home.get_libs(module_dir, import_libs) == false) {
                log::warn("Failed getting libraries from import module directory: %s", module_dir);
                continue;
            }
            for (const auto& lib : import_libs) {
                log::trace("Adding imported library for linking: %s", lib);
                libs += string(" -l") + lib;
            }
        }

        // Additional library paths
        for (const string& lib_dir : libdirs) {
            log::trace("Adding additional library directory: %s", lib_dir);
            lib_dirs += string(" -L") + lib_dir;
        }

        for (const string& lib : libraries) {
            log::trace("Adding additional library: %s", lib);
            libs += string(" -l") + lib;
        }

        log::debug("Configured library directories: %s", lib_dirs);
        log::debug("Configured libraries: %s", libs);
        log::trace("Finished configuring library directories and libraries");
        return true;
    }

    bool build_tools::configure_inc_dir(const string_list& imports, const string_list& includes)
    {
        log::trace("Configuring include directories");
        
        // Add project include paths
        log::trace("Adding project include directory: %s", the_home.get_active_project_inc_dir());
        inc_dirs = string("-I") + the_home.get_active_project_inc_dir();

        // Add import include paths
        for (const string& import : imports) {
            
            auto module_inc_dir = the_home.get_module_inc_dir(import);
            log::trace("Adding import module include directory: %s", module_inc_dir);
            inc_dirs += string(" -I") + module_inc_dir;

            // Should we also add the project include directory for each import? 
            // This is commented out for now.
            // auto project_inc_dir = the_home.get_project_inc_dir(import);
            // inc_dirs += string(" -I") + project_inc_dir;
        }

        // Additional include paths
        for (const string& include : includes) {
            log::trace("Adding additional include directory: %s", include);
            inc_dirs += string(" -I") + include;
        }

        log::debug("Configured include directories: %s", inc_dirs);
        log::trace("Finished configuring include directories");
        return true;
    }

    bool build_tools::build_libs()  
    {
        log::trace("Trying to build library targets...");

        auto project_dir = the_home.get_active_project_dir();
        auto project_name = the_home.get_active_project_name();

        log::trace("Under '%s'", project_dir);
        log::trace("Check if '/lib' exist");
        
        // Check if the "lib" directory exist
        // if it is call the build_lib() function using the "/lib" sub_dir
        if (fs::exists(project_dir + "/lib")) {
            log::trace("Found '/lib' directory, building library");
            if (build_lib("lib", project_name) == false) {
                log::trace("Failed to build library for '/lib'");
                return false;
            } else {
                log::trace("Successfully built library for '/lib'");                
            }
        }

        // Check if the "libs" directory exist
        // if it is call the build_lib() function for all sub directory under "/libs" 
        log::trace("Check if '/libs' exist");
        if (fs::exists(project_dir + "/libs")) {
            log::trace("Found '/libs' directory, iterating over subdirectories");
            for (const auto& entry : fs::directory_iterator(project_dir + "/libs")) {
                if (entry.is_directory()) {
                    auto sub_dir = entry.path().filename().string();
                    log::trace("Building library for subdirectory: '%s'", sub_dir);
                    if (build_lib("libs/" + sub_dir, sub_dir) == false) {
                        log::trace("Failed to build library for subdirectory: '%s'", sub_dir);
                        return false;
                    }
                }
            }
        }

        log::trace("Finished building library targets");
        return true;
    }

    bool build_tools::build_lib(const string& sub_dir, const string& name)  
    {
        log::debug("Building library: %s from subdirectory: %s", name, sub_dir);

        // create plan
        build_plan plan;

        if(plan_dir_compilation(plan, sub_dir)==false)
            return false;

        if(plan_lib_creation(plan, sub_dir, name)==false)
            return false;
        log::debug("Planned %d files for compilation and library creation", plan.size());

        // run the plan
        if(run_build_plan(plan) == false)
            return false;

        log::trace("Successfully built library: %s", name);
        log::trace("Adding library to link flags: %s", name);
        libs += (string(" -l") + name);
        return true;
    }

    bool build_tools::build_app(const string& sub_dir, const string& name)  const
    {
        log::debug("Building application: %s from subdirectory: %s", name, sub_dir);
        // create plan
        build_plan plan;

        if(plan_dir_compilation(plan, sub_dir)==false)
            return false;   
        log::debug("Planned %d files for compilation and binary linking", plan.size());

        if(plan_app_linking(plan, sub_dir, name)==false)
            return false;
            
        // run the plan
        if (run_build_plan(plan) == false)
            return false;

        log::trace("Successfully built application: %s", name);
        return true;
    }

    bool build_tools::build_apps() const
    {
        log::trace("Trying to build application targets...");

        auto project_dir = the_home.get_active_project_dir();
        auto project_name = the_home.get_active_project_name();

        log::trace("Under '%s'", project_dir);
        log::trace("Check if '/app' exist");
        // Check if the "lib" directory exist
        // if it is call the build_lib() function using the "/lib" sub_dir
        if (fs::exists(project_dir + "/app")) {
            log::trace("Found '/app' directory, building application");
            return build_app("app", project_name);
        }

        log::trace("Under '%s'", project_dir);
        log::trace("Check if '/apps' exist");
        // Check if the "apps" directory exist
        // if it is call the build_app() function for all sub directory under "/apps" 
        if (fs::exists(project_dir + "/apps")) {
            log::trace("Found '/apps' directory, iterating over subdirectories");
            for (const auto& entry : fs::directory_iterator(project_dir + "/apps")) {
                if (entry.is_directory()) {
                    auto sub_dir = entry.path().filename().string();
                    log::trace("Building application for subdirectory: '%s'", sub_dir);
                    if (build_app("apps/" + sub_dir, sub_dir) == false) {
                        log::trace("Failed to build application for subdirectory: '%s'", sub_dir);
                        return false;
                    }
                }
            }
        }

        return true;
    }

    bool build_tools::build_tests()  const
    {
        log::debug("Trying to build test targets...");

        // create plan
        build_plan plan;
        plan_dir_compilation(plan, "tests");
        log::debug("Planned %d files for compilation", plan.size());
        run_build_plan(plan);

        plan.clear();
        plan_tests_linking(plan);
        if(run_build_plan(plan) == false)
            return false;

        log::trace("Successfully built test targets");
        return true;
    }

    bool build_tools::need_recompile(const build_task& task, const file_ts& dir_ts) const
    {
        // If the target file does not exist, we need to recompile
        if(fs::exists(task.target)==false)
            return true;

        // Check the last write time of the source
        auto source_ts = fs::last_write_time(task.source);
        auto target_ts = fs::last_write_time(task.target);

        log::trace("Source timestamp: %s", ltd::string_ts(source_ts));
        log::trace("Target timestamp: %s", ltd::string_ts(target_ts));

        // If the source file has been modified, we need to recompile
        if(source_ts > target_ts || lib_header_ts > target_ts || dir_ts > target_ts)
            return true;
        return false;
    }

    bool build_tools::plan_lib_creation(build_plan& plan, const string& sub_dir, const string& name) const
    {
        log::trace("Planning library creation for: lib%s.a", name);

        auto dir = the_home.get_active_build_dir(debug) + "/" + sub_dir;
        if (!fs::exists(dir)) {
            log::fatal("Missing build directory: %s", dir);
            return false;
        }

        build_task task;
        task.type = ARCHIVE;
        task.source = dir;
        task.target = the_home.get_active_build_dir(debug) + "/lib" + name + ".a";
        task.message = "Creating library " + task.target + "...";
        plan.push_back(task);

        return true;
    }

    bool build_tools::plan_app_linking(build_plan& plan, const string& sub_dir, const string& name) const
    {
        log::trace("Planning application creation for: %s", name);

        auto dir = the_home.get_active_build_dir(debug) + "/" + sub_dir;
        if (!fs::exists(dir)) {
            log::fatal("Missing build directory: %s", dir);
            return false;
        }

        build_task task;
        task.type = LINK;
        task.source = dir + "/*.o";
        task.target = the_home.get_active_build_dir(debug) + "/" + name;
        task.message = "Linking application '" + name + "'...";
        plan.push_back(task);

        return true;
    }

    bool build_tools::plan_tests_linking(build_plan& plan) const
    {
        log::trace("Planning tests linking");
        auto dir = the_home.get_active_build_dir(debug) + "/tests";
        if (!fs::exists(dir)) {
            log::fatal("Missing build directory for tests: %s", dir);
            return false;
        }
        log::trace("Looking for object files in: %s", dir);

        bool warn = true;
        for (const auto& entry : fs::directory_iterator(dir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".o") {
                log::trace("Found object file for tests: %s", entry.path().filename().string());

                build_task task;
                task.type = LINK;
                task.source = entry.path().c_str();
                string file_name = entry.path().filename().replace_extension("").c_str();
                task.target = the_home.get_active_build_dir(debug) + "/tests/" + file_name;
                task.message = "Linking tests binary " + file_name + "...";
                plan.push_back(task);
                warn = false; 
            }
        }
        if (warn) {
            log::warn("No object files found for tests in: %s", dir);
        }
        return true;
    }

    bool build_tools::plan_dir_compilation(build_plan& plan, const string& sub_dir) const 
    {
        log::trace("Planning compilation for directory: %s", sub_dir);
        
        auto source_dir = the_home.get_active_project_dir() + "/" + sub_dir; 
        log::trace("Source directory: %s", source_dir);
        if (!fs::exists(source_dir)) {
            log::fatal("Missing source directory to build: %s", source_dir); 
            return false; 
        }

        auto target_dir = the_home.get_active_build_dir(debug) + "/" + sub_dir; 
        log::trace("Target build directory: %s", target_dir);
        if (!fs::exists(target_dir)) {
            log::trace("Cannot find target build directory. Creating directory '%s'", target_dir);
            fs::create_directories(target_dir);
        }

        log::trace("Looking for .ignore file...");
        auto ignore_file_path = source_dir + "/.ignore";
        ignore_filter filter(source_dir + "/.ignore");
        if (fs::exists(ignore_file_path)) {
            log::trace("Found .ignore file. Loading...");
            if(filter.load_patterns() == false) 
                log::warn("Failed to load .ignore");
        }
        
        auto dir_header_ts = the_home.get_headers_write_time(source_dir);
        log::trace("Library header timestamp: %s", ltd::string_ts(lib_header_ts));
        log::trace("Directory header timestamp: %s", ltd::string_ts(dir_header_ts));
        for (auto dir_entry : fs::directory_iterator(source_dir)) {
            if (dir_entry.is_directory()) 
                continue;

            auto ext = dir_entry.path().extension();
            if (ext == ".cpp" || ext == ".cc" || ext == ".cxx") {
                
                string file_name = dir_entry.path().filename();
                if (filter.is_ignored(file_name)) {
                    log::trace("Ignoring file: %s", file_name);
                    continue;
                }

                build_task task;

                task.type = COMPILE;
                task.source = dir_entry.path().c_str();

                string obj_file = dir_entry.path().filename().replace_extension(".o");
                task.target = target_dir + "/" + obj_file.c_str();

                string src_file = dir_entry.path().filename().c_str();
                task.message = "Compiling " + src_file + "...";

                log::trace("Source: %s\nTarget: %s", task.source, task.target);
                if (need_recompile(task, dir_header_ts)) {
                    log::debug("Adding '%s' for compilation...", file_name);
                    plan.push_back(task);
                }                                                 
            }
        }

        return true;
    }

    bool build_tools::run_build_plan(build_plan& plan) const
    {
        if (plan.empty()) {
            log::info("Nothing to build. All targets are up to date.");
            return true;
        }

        bool compiled=false;
        for (const auto& task : plan) {
            bool success = false;

            switch (task.type) {
                case COMPILE:
                    {
                        compiled = true;
                        log::info(task.message);
                        log::trace("  Source: %s\n  Target: %s", task.source, task.target);
                        auto command = fmt::sprintf("%s %s -c %s -o %s %s", cpp, cppflags, task.source, task.target, inc_dirs);
                        log::trace(command);
                        auto result = std::system(command.c_str());
                        success = (result == 0);
                    }                    
                    break;
                case LINK:
                    {
                        log::trace("  Source: %s\n  Target: %s", task.source, task.target);
                        if (compiled == false && fs::exists(task.target) == true) {
                            log::debug("No object files were compiled. Skipping linking step.");
                            continue;
                        }
                        log::info(task.message);
                        auto command = fmt::sprintf("%s %s -o %s %s %s", cpp, task.source, task.target, libs, lib_dirs);
                        log::trace(command);
                        auto result = std::system(command.c_str());
                        success = (result == 0);
                    }
                    break;
                case ARCHIVE:
                    {
                        log::trace("  Source: %s\n  Target: %s", task.source, task.target);

                        if (compiled == false && fs::exists(task.target) == true) {
                            log::debug("No object files were compiled. Skipping archiving library step.");
                            continue;
                        }

                        log::info(task.message);
                        auto command = fmt::sprintf("ar rcs %s %s/*.o", task.target, task.source);
                        log::trace(command);
                        auto result = std::system(command.c_str());
                        success = (result == 0);
                    }
                    break;
                default:
                    log::fatal("Unknown build task type");
                    return false;
            }

            if (!success) {
                log::fatal("Failed to execute build task: %s", task.message);
            }
        }
        return true;
    }

    bool build_tools::compile_file(const string& source, const string& target) const
    {
        auto command = fmt::sprintf("%s %s -c %s -o %s %s", cpp, cppflags, source, target);
        log::trace(command);
        auto result = std::system(command.c_str());
        return result == 0;
    }
} // namespace ltd
