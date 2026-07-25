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
        log::debug("lib_header_ts: %s", ltd::string_ts(lib_header_ts));

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
        if (configure_lib_dir(info.imports, info.libs) == false) {
            log::fatal("Failed configuring lib paths");
            return false;
        }

        return true;
    }

    bool build_tools::configure_lib_dir(const string_list& imports, const string_list& libraries) 
    {
        // Add the active builds directory to the library search path
        auto project_build_dir = the_home.get_active_build_dir(debug);
        lib_dirs = string("-L") + project_build_dir;

        // lets list all the libraries in the active builds directory and add them to the libs list
        string_list build_libs;
        if (the_home.get_libs(project_build_dir, build_libs) == false) {
            log::fatal("Failed getting libraries from active builds directory: %s", project_build_dir);
            return false;
        }

        for (const auto& lib : build_libs) {
            libs += string(" -l") + lib;
        }

        // Add import library paths
        for (const string& import : imports) {
            auto module_dir = the_home.get_module_dir(import);
            lib_dirs += string(" -L") + module_dir;

            string_list import_libs;
            if (the_home.get_libs(module_dir, import_libs) == false) {
                log::warn("Failed getting libraries from import module directory: %s", module_dir);
                continue;
            }
            for (const auto& lib : import_libs) {
                libs += string(" -l") + lib;
            }
        }

        // Additional library paths
        for (const string& lib_dir : libraries) {
            lib_dirs += string(" -L") + lib_dir;
        }

        return true;
    }

    bool build_tools::configure_inc_dir(const string_list& imports, const string_list& includes)
    {
        // Add project include paths
        inc_dirs = string("-I") + the_home.get_active_project_inc_dir();

        // Add import include paths
        for (const string& import : imports) {
            
            auto module_inc_dir = the_home.get_module_inc_dir(import);
            inc_dirs += string(" -I") + module_inc_dir;

            // Should we also add the project include directory for each import? 
            // This is commented out for now.
            // auto project_inc_dir = the_home.get_project_inc_dir(import);
            // inc_dirs += string(" -I") + project_inc_dir;
        }

        // Additional include paths
        for (const string& include : includes) {
            inc_dirs += string(" -I") + include;
        }

        return true;
    }

    bool build_tools::build_libs()  const
    {
        log::trace("Entering build_libs()");

        auto project_dir = the_home.get_active_project_dir();
        auto project_name = the_home.get_active_project_name();

        log::trace("Under '%s'", project_dir);
        log::trace("Check if '/lib' exist");
        
        // Check if the "lib" directory exist
        // if it is call the build_lib() function using the "/lib" sub_dir
        if (fs::exists(project_dir + "/lib")) {
            log::trace("Found '/lib' directory, building library");
            return build_lib("lib", project_name);
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

        return true;
    }

    bool build_tools::build_lib(const string& sub_dir, const string& name)  const
    {
        // create plan
        build_plan plan;

        if(plan_dir_compilation(plan, sub_dir)==false)
            return false;

        if(plan_lib_creation(plan, sub_dir, name)==false)
            return false;

        // run the plan
        run_build_plan(plan);
        return true;
    }

    bool build_tools::build_app(const string& sub_dir, const string& name)  const
    {
        // create plan
        build_plan plan;

        if(plan_dir_compilation(plan, sub_dir)==false)
            return false;   

        if(plan_app_linking(plan, sub_dir, name)==false)
            return false;
            
        // run the plan
        run_build_plan(plan);

        return true;
    }

    bool build_tools::build_apps() const
    {
        log::trace("Entering build_apps()");

        auto project_dir = the_home.get_active_project_dir();

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
        // create plan
        build_plan plan;
        plan_dir_compilation(plan, "tests");
        plan_tests_linking(plan);
        
        // run the plan
        run_build_plan(plan);
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

        log::trace("  Source ts: %s", ltd::string_ts(source_ts));
        log::trace("  Target ts: %s", ltd::string_ts(target_ts));
        log::trace("  Lib header ts: %s", ltd::string_ts(lib_header_ts));
        log::trace("  Dir header ts: %s", ltd::string_ts(dir_ts));

        // If the source file has been modified, we need to recompile
        if(source_ts > target_ts || lib_header_ts > target_ts || dir_ts > target_ts)
        {
            log::debug("Recompilation needed for: %s", task.source);
            return true;
        }
        else
            log::debug("No recompilation needed for: %s", task.source);
        
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
        task.message = "Linking application " + task.target + "...";
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

        for (const auto& entry : fs::directory_iterator(dir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".o") {
                log::trace("Found object file for tests: %s", entry.path().filename().string());

                build_task task;
                task.type = LINK;
                task.source = entry.path().c_str();
                string file_name = entry.path().filename().replace_extension("").c_str();
                task.target = the_home.get_active_build_dir(debug) + "/" + file_name;
                task.message = "Linking tests binary " + file_name + "...";
                plan.push_back(task);
            }
        }
        return true;
    }

    bool build_tools::plan_dir_compilation(build_plan& plan, const string& sub_dir) const 
    {
        log::trace("Planning compilation for directory: %s", sub_dir);

        auto source_dir = the_home.get_active_project_dir() + "/" + sub_dir; 
        if (!fs::exists(source_dir)) {
            log::fatal("Missing source directory to build: %s", source_dir); 
            return false; 
        }

        auto target_dir = the_home.get_active_build_dir(debug) + "/" + sub_dir; 
        if (!fs::exists(target_dir)) {
            fs::create_directories(target_dir);
        }

        ignore_filter filter(source_dir + "/.ignore");
        if(filter.load_patterns() == false) {
            log::trace("Failed to load ignore patterns from: %s", source_dir);
        }

        auto dir_header_ts = the_home.get_headers_write_time(source_dir);
        
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
