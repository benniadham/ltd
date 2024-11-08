#include "compiler.hpp"

#include <filesystem>

namespace fs = std::filesystem;

#include "../inc/fmt.hpp"

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

        void Cpp::add_inc_path(const String& path)
        {
            inc_paths.push_back(path);
        }

        void Cpp::add_lib_path(const String& path)
        {
            lib_paths.push_back(path);
        }

        void Cpp::add_library(const String& lib_name)
        {
            libraries.push_back(lib_name);
        }

        String Cpp::get_compiler() const
        {
            return compiler;
        }

        void Cpp::set_compiler(const String& compiler_command)
        {
            compiler = compiler_command;
        }

        String Cpp::get_standard() const
        {
            return standard;
        }

        void Cpp::set_standard(const String& cpp_standard)
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

        void Cpp::compile_file(const String& src, const String& dst) const
        {
            auto command = fmt::sprintf("%s -std=%s -c %s -o %s", compiler, standard, src, dst);
            fmt::println(command);
            std::system(command.c_str());
        }

        void Cpp::compile_files(const String& src_dir, const String& obj_dir) const
        {
            Entries entries;

            for(const auto& dir_entry : fs::directory_iterator(src_dir)) 
            {
                auto ext = dir_entry.path().extension();

                if (ext == ".cpp" || ext == ".cc" || ext == ".cxx") {
                    String src_file = dir_entry.path().c_str();

                    String obj_file = dir_entry.path().filename().replace_extension(".o");
                    
                    obj_file = obj_dir + "/" + obj_file.c_str();

                    Entry entry = std::make_pair(src_file, obj_file);
                    entries.push_back(entry);
                }
            }

            for(int i=0; i<entries.size(); i++) {
                fmt::println("Compiling %d of %d...", i+1, entries.size());
                compile_file(entries[i].first, entries[i].second);
            }
        }

        void Cpp::build_lib(const String& obj_dir, const String& lib_target) const
        {
            String obj_files;
            
            for(const auto& dir_entry : fs::directory_iterator(obj_dir)) {
                auto ext = dir_entry.path().extension();
                if (ext == ".o") {
                    String obj_file = dir_entry.path().c_str();
                    obj_files += obj_file;
                    obj_files += " ";
                }
            }

            auto link_command = "ar rcs " + lib_target + " " + obj_files;

            fmt::println(link_command.c_str());
            std::system(link_command.c_str());
        }

        void Cpp::build_app(const String& obj_dir, const String& target) const
        {
            String obj_files;
            
            for(const auto& dir_entry : fs::directory_iterator(obj_dir)) {
                auto ext = dir_entry.path().extension();
                if (ext == ".o") {
                    String obj_file = dir_entry.path().c_str();
                    obj_files += obj_file;
                    obj_files += " ";
                }
            }

            // auto out_dir = get_active_build_path(debug);
            String lib_paths_flags;
            for(auto lib_path : lib_paths) 
                lib_paths_flags += "-L" + lib_path;

            String lib_flags;
            for(auto library : libraries) 
                lib_flags += "-l" + library;

            auto link_command = fmt::sprintf("%s -o %s %s %s %s", 
                                compiler, target, obj_files, lib_paths_flags, lib_flags);

            fmt::println(link_command.c_str());
            std::system(link_command.c_str());
        }
    } // namespae sdk
} // namespace ltd