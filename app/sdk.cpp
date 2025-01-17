#include "sdk.hpp"

#include <chrono>
#include <ctime>
#include <format>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

#include "../inc/ltd/cli.hpp"

#include "compiler.hpp"

namespace ltd
{
    namespace sdk
    {
        void parse_flags(cli_args& args)
        {
            args.add_param("verbosity", "1", "Specifies verbosity level 1, 2, 3 or 4");
            args.add_param("std", "", "Specifies cpp standards");

            args.add_flag('v', 0, "Sets verbosity level 1-4");
            args.add_flag('g', 0, "Debug mode");

            args.add_command("ls",  CMD_LS, "List all projects in the workspace");
            args.add_command("pwd", CMD_PWD, "Show currect active project");
            args.add_command("cd",  CMD_CD, "Change project directory");
            
            args.add_command("build", CMD_BUILD, "Build the current active project");
            args.add_command("clean", CMD_CLEAN, "Clean the current active project");
            args.add_command("test",  CMD_TEST, "Run tests");
            args.add_command("help",  CMD_HELP, "Show this help");

            args.parse();
        }

        bool is_homepath_set()
        {
            return getenv("LTD_HOME") == NULL ? false : true;
        }

        string get_homepath()
        {
            // Check LTD_HOME variable
            auto env_home = getenv("LTD_HOME"); 

            if(env_home == NULL) {
                return "";
            }

            return env_home;
        }

        string get_active_project()
        {
            string file_name = get_homepath() + "/.project";

            if(fs::exists(file_name)) {
                std::ifstream file(file_name);
                string project;

                file >> project;

                return project; 
            }

            return "";
        }

        void set_active_project(const string& project)
        {
            string file_name = get_homepath() + "/.project";

            std::ofstream file(file_name);

            file << project << std::endl;

            file.close();
        }

        string get_active_project_path()
        {
            return get_homepath() + "/projects/" + get_active_project();
        }

        string get_builds_path()
        {
            return get_homepath() + "/builds";
        }

        string get_active_build_path(bool debug)
        {
            string build_mode = debug ? "/debug" : "/release";
            return  get_builds_path() + "/" + get_active_project() + build_mode; 
        }

        void get_projects_list(string_list& projects)
        {
            string home_path = get_homepath();
            string project_path = home_path + "/projects";

            if (!fs::exists(project_path))
                return;

            for(const auto& dir_entry : fs::directory_iterator(project_path)) 
            {
                if (fs::is_directory(dir_entry)) {
                    string dir = dir_entry.path();
                    size_t index = dir.find_last_of('/');

                    string prj = dir.substr(index+1);

                    if(prj.at(0) == '.')
                        continue;

                    projects.push_back(prj);
                }
            } 
        }

        void list_project_dir(string_list& dirs)
        {
            for(const auto& dir_entry : fs::directory_iterator(sdk::get_active_project_path())) {
                if (fs::is_directory(dir_entry)) {
                    string dir = dir_entry.path();
                    size_t index = dir.find_last_of('/');

                    string prj = dir.substr(index+1);

                    if(prj.at(0) != '.')
                        dirs.push_back(prj);
                }
            } 

            struct
            {
                bool operator()(const string& a, const string& b) const { return a < b; }
            }
            customLess;

            // Sort dirs to prioritize library builds first
            std::sort(dirs.begin(), dirs.end(), [] (const string& a, const string& b) 
                                                        {
                                                            if (a.find("lib") == 0)
                                                                return true;
                                                            return false;
                                                        });
        }

        void build_dir(const string& name, const string& sub_dir, bool debug)
        {
            string build_mode = debug ? "/debug" : "/release";

            cli::debug("Build mode: %s", build_mode);

            // Get source file path
            string src_path = sdk::get_active_project_path() + sub_dir;
            cli::debug("Source path: %s", src_path);

            // Determine object file path
            string dst_path = sdk::get_builds_path();
            if (fs::exists(dst_path) == false) {
                fs::create_directory(dst_path);
            }
            cli::debug("Build path: %s", dst_path);
            
            dst_path += "/" + sdk::get_active_project();
            if (fs::exists(dst_path) == false) {
                fs::create_directory(dst_path);
            }

            string build_dir = dst_path + build_mode; 
            cli::debug("Build path: %s", build_dir);
            if (fs::exists(build_dir) == false) {
                fs::create_directory(build_dir);
            }

            string obj_path = build_dir + sub_dir;
            cli::debug("Build object path: %s", obj_path);
            if (fs::exists(obj_path) == false) {
                fs::create_directory(obj_path);
            }

            Cpp cc;

            cc.compile_files( src_path, obj_path);

            if (sub_dir.find("/lib")==0) {
                string target = build_dir + "/lib" + name + ".a";
                cc.build_lib(obj_path, target);
            } else {
                string target = build_dir + "/" + name;
                cc.add_lib_path(build_dir);
                cc.add_library(name);
                cc.build_app(obj_path, target);
            }
        }

        fs::file_time_type get_dir_write_time(const string& path)
        {
            fs::path dir_path(path);
            fs::directory_entry dir(dir_path);
            return dir.last_write_time();
        }

        template <typename TP>
        std::time_t to_time_t(TP tp)
        {
            using namespace std::chrono;
            auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
                    + system_clock::now());
            return system_clock::to_time_t(sctp);
        }

        string file_time_to_string(const fs::file_time_type& timestamp)
        {
            std::time_t tt = to_time_t(timestamp);
            std::tm *gmt = std::gmtime(&tt);
            std::stringstream buffer;

            buffer << std::put_time(gmt, "\t%a, %d %b %Y %H:%M:%S");

            return buffer.str();
        }

        void clear_dir(const fs::path& path)
        {
            for(const auto& dir_entry : fs::directory_iterator(path))  {
                if (dir_entry.is_directory())
                    clear_dir(dir_entry);
                fs::remove(dir_entry);
            }
        }

        void clean_project(bool debug)
        {
            string path = get_active_build_path(debug);
            fs::path dir_path(path);
            clear_dir(dir_path);
        }
    }
}