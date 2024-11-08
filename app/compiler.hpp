#ifndef _LTD_INCLUDE_COMPILER_HPP_
#define _LTD_INCLUDE_COMPILER_HPP_

#include "../inc/stddef.hpp"

namespace ltd
{
    namespace sdk
    {
        class Cpp
        {
        private:
            String compiler = "g++";
            String standard = "c++17";
            bool debug = false;

            StringList inc_paths;
            StringList lib_paths;
            StringList libraries;

        public:
            Cpp();
            Cpp(const Cpp& other);

            void add_inc_path(const String& path);
            void add_lib_path(const String& path);
            void add_library(const String& lib_name);

            String get_compiler() const;
            void set_compiler(const String& compiler_command);

            String get_standard() const;
            void set_standard(const String& cpp_standard);

            bool is_debug() const;
            void set_debug(bool debug_mode);

            using Entry   = std::pair<String,String>;
            using Entries = std::vector<Entry>;

            /**
             * @brief
             * Compile all files under a directory into .o files.
             */
            void compile_files(const String& src_dir, const String& obj_dir) const;

            /**
             * @brief
             * Compile a singular C++ source file
             */
            void compile_file(const String& src, const String& dst) const;

            /**
             * @brief
             * Create .a library file using .o files under the specified object directory.
             */
            void build_lib(const String& obj_dir, const String& lib_target) const;

            /**
             * @brief
             * Link .o files into an executable
             */
            void build_app(const String& obj_dir, const String& target) const;
        };
    } // namespace sdk
} // namespace ltd

#endif // _LTD_INCLUDE_COMPILER_HPP_