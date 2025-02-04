#include "compiler.hpp"

#include <filesystem>

namespace fs = std::filesystem;

#include "../inc/ltd/fmt.hpp"
#include "../inc/ltd/cli.hpp"

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
            cli::trace(command);
            auto result = std::system(command.c_str());
        }

        void Cpp::compile_files(const string& src_dir, const string& obj_dir) const
        {
            Entries entries;

            for(const auto& dir_entry : fs::directory_iterator(src_dir)) 
            {
                auto ext = dir_entry.path().extension();

                if (ext == ".cpp" || ext == ".cc" || ext == ".cxx") {
                    string src_file = dir_entry.path().c_str();

                    string obj_file = dir_entry.path().filename().replace_extension(".o");
                    
                    obj_file = obj_dir + "/" + obj_file.c_str();

                    Entry entry = std::make_pair(src_file, obj_file);
                    entries.push_back(entry);
                }
            }

            for(int i=0; i<entries.size(); i++) {
                fs::path file = entries[i].first;
                cli::debug("Compiling %d of %d... %s", i+1, entries.size(), file.filename());
                compile_file(entries[i].first, entries[i].second);
            }
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
            cli::info("Creating lib: %s", target_path.filename());

            auto link_command = "ar rcs " + lib_target + " " + obj_files;

            cli::trace(link_command.c_str());
            auto result = std::system(link_command.c_str());
        }

        void Cpp::build_app(const string& obj_dir, const string& target) const
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
                cli::debug("Add lib path -L%s", lib_path);
                lib_paths_flags += "-L" + lib_path + " ";
            }

            string lib_flags;
            for(auto library : libraries) {
                cli::debug("Add lib -l%s", library);
                lib_flags += "-l" + library + " ";
            }

            fs::path target_path = target;
            cli::info("Linking app: %s", target_path.filename());

            auto link_command = fmt::sprintf("%s -o %s %s %s %s", 
                                compiler, target, obj_files, lib_paths_flags, lib_flags);

            cli::trace(link_command.c_str());
            auto result = std::system(link_command.c_str());
        }

        void Cpp::build_tests(const string& obj_dir, const string& target) const
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

                    cli::info("Linking test unit: %s", test_exec);

                    auto link_command = fmt::sprintf("%s -o %s%s %s %s %s", 
                        compiler, target, test_exec, obj_file, lib_paths_flags, lib_flags);

                    cli::trace(link_command.c_str());
                    auto result = std::system(link_command.c_str());
                }
            }
        }
    } // namespae sdk
} // namespace ltd