#include "compiler.hpp"

#include <filesystem>

namespace fs = std::filesystem;

#include "../inc/ltd/fmt.hpp"
#include "../inc/ltd/cli.hpp"
#include "../inc/ltd/log.hpp"

namespace ltd
{
    namespace sdk
    {
        Cpp::Cpp()
        {

        }

        Cpp::Cpp(const Cpp& other)
        {
            compiler = other.compiler;
            standard = other.standard;
            debug    = other.debug;
        }

        void Cpp::add_inc_path(const string& path)
        {
            inc_paths.push_back(path);
        }

        void Cpp::add_lib_path(const string& path)
        {
            lib_paths.push_back(path);
        }

        void Cpp::add_library(const string& lib_name)
        {
            libraries.push_back(lib_name);
        }

        string Cpp::get_compiler() const
        {
            return compiler;
        }

        void Cpp::set_compiler(const string& compiler_command)
        {
            compiler = compiler_command;
        }

        string Cpp::get_standard() const
        {
            return standard;
        }

        void Cpp::set_standard(const string& cpp_standard)
        {
            standard = cpp_standard;
        }

        bool Cpp::is_debug() const
        {
            return debug;
        }

        void Cpp::set_debug(bool debug_mode)
        {
            debug = debug_mode;
        }

        void Cpp::compile_file(const string& src, const string& dst) const
        {
            string inc_flags;
            for (auto inc_path : inc_paths) {
                inc_flags += " -I" + inc_path;
            }

            auto command = fmt::sprintf("%s -std=%s -c %s -o %s %s", compiler, standard, src, dst, inc_flags);
            log::trace(command);
            auto result = std::system(command.c_str());
        }

        int Cpp::compile_files(const string& src_dir, const string& obj_dir, const fs::file_time_type& entry_time) const
        {
            Entries entries;
            int counter = 0;

            log::trace("Entering '%s'", src_dir);

            // Collecting dirty source files for compilation
            for(const auto& dir_entry : fs::directory_iterator(src_dir)) 
            {
                auto ext = dir_entry.path().extension();

                if (ext == ".cpp" || ext == ".cc" || ext == ".cxx") {
                    string src_file_path = dir_entry.path().c_str();
                    string src_file = dir_entry.path().filename().c_str();

                    string obj_file = dir_entry.path().filename().replace_extension(".o");
                    
                    string obj_file_path = obj_dir + "/" + obj_file.c_str();

                    bool need_compile = true;
                    if(std::filesystem::exists(obj_file_path)) {
                        auto srctime = std::filesystem::last_write_time(src_file_path);
                        auto objtime = std::filesystem::last_write_time(obj_file_path);
                        
                        if(srctime > objtime) {
                            log::trace("'%s' is edited...", src_file);
                            need_compile = true;
                        } else if (objtime < entry_time) {
                            need_compile = true;
                            log::trace("'%s' header files is updated...", src_file);
                        } else {
                            need_compile = false;
                            log::trace("'%s' is up-to-date...", obj_file);
                        }
                    } else {
                        log::trace("'%s' does not exist...", obj_file);
                    }
                    
                    if(need_compile) {
                        Entry entry = std::make_pair(src_file_path, obj_file_path);
                        entries.push_back(entry);
                    } 
                }
            }

            if(entries.size() == 0)
                log::info("No files found for compilation...");
 
            for(int i=0; i<entries.size(); i++) {
                fs::path file = entries[i].first;
                log::info("Compiling %d of %d... %s", i+1, entries.size(), file.filename());
                compile_file(entries[i].first, entries[i].second);

                counter++;
            }

            log::trace("Exiting '%s'", src_dir);
            return counter;
        }

        void Cpp::build_lib(const string& obj_dir, const string& lib_target) const
        {
            string obj_files;
            
            for(const auto& dir_entry : fs::directory_iterator(obj_dir)) {
                auto ext = dir_entry.path().extension();
                if (ext == ".o") {
                    string obj_file = dir_entry.path().c_str();
                    obj_files += obj_file;
                    obj_files += " ";
                }
            }

            fs::path target_path = lib_target;
            log::info("Creating lib: %s", target_path.filename());

            auto link_command = "ar rcs " + lib_target + " " + obj_files;

            log::trace(link_command.c_str());
            auto result = std::system(link_command.c_str());
        }

        void Cpp::link_app(const string& obj_dir, const string& target) const
        {
            string obj_files;
            
            for(const auto& dir_entry : fs::directory_iterator(obj_dir)) {
                auto ext = dir_entry.path().extension();
                if (ext == ".o") {
                    string obj_file = dir_entry.path().c_str();
                    obj_files += obj_file;
                    obj_files += " ";
                }
            }

            string lib_paths_flags;
            for(auto lib_path : lib_paths) {
                log::debug("Add lib path -L%s", lib_path);
                lib_paths_flags += "-L" + lib_path + " ";
            }

            string lib_flags;
            for(auto library : libraries) {
                log::debug("Add lib -l%s", library);
                lib_flags += "-l" + library + " ";
            }

            fs::path target_path = target;
            log::info("Linking app: %s", target_path.filename());

            auto link_command = fmt::sprintf("%s -o %s %s %s %s", 
                                compiler, target, obj_files, lib_paths_flags, lib_flags);

            log::trace(link_command.c_str());
            auto result = std::system(link_command.c_str());
        }

        void Cpp::link_tests(const string& obj_dir, const string& target) const
        {
            string lib_paths_flags;
            for(auto lib_path : lib_paths) 
                lib_paths_flags += "-L" + lib_path + " ";

            string lib_flags;
            for(auto library : libraries) 
                lib_flags += "-l" + library + " ";

            for(const auto& dir_entry : fs::directory_iterator(obj_dir)) {
                auto ext = dir_entry.path().extension();
                if (ext == ".o") {
                    string obj_file = dir_entry.path().c_str();
                    string test_exec = dir_entry.path().filename().replace_extension("");

                    bool need_linking = true;
                    if(std::filesystem::exists(target+test_exec)) {
                        auto srctime = std::filesystem::last_write_time(obj_file);
                        auto objtime = std::filesystem::last_write_time(target+test_exec);

                        need_linking = srctime > objtime;
                    }

                    if(need_linking) {
                        log::info("Linking test unit: '%s'", test_exec);

                        auto link_command = fmt::sprintf("%s -o %s%s %s %s %s -lstdc++exp", 
                            compiler, target, test_exec, obj_file, lib_paths_flags, lib_flags);

                        log::trace(link_command.c_str());
                        auto result = std::system(link_command.c_str());
                    } else {
                        log::info("Unit test is up-to-date: '%s'", test_exec);
                    }
                }
            }
        }
    } // namespace sdk
} // namespace ltd