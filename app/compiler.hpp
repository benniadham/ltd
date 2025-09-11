#ifndef _LTD_INCLUDE_COMPILER_HPP_
#define _LTD_INCLUDE_COMPILER_HPP_

#include "../inc/ltd/stddef.hpp"

namespace ltd
{
    namespace sdk
    {
        class Cpp
        {
        private:
            string compiler = "g++";
            string standard = "c++17";
            bool debug = false;

            string_list inc_paths;
            string_list lib_paths;
            string_list libraries;

        public:
            Cpp();
            Cpp(const Cpp& other);

            void add_inc_path(const string& path);
            void add_lib_path(const string& path);
            void add_library(const string& lib_name);

            string get_compiler() const;
            void set_compiler(const string& compiler_command);

            string get_standard() const;
            void set_standard(const string& cpp_standard);

            bool is_debug() const;
            void set_debug(bool debug_mode);

            using Entry   = std::pair<string,string>;
            using Entries = std::vector<Entry>;

            /**
             * @brief
             * Compile all files under a directory into .o files.
             * 
             * @returns Number of files compiled.
             */
            int compile_files(const string& src_dir, const string& obj_dir) const;

            /**
             * @brief
             * Compile a singular C++ source file
             */
            void compile_file(const string& src, const string& dst) const;

            /**
             * @brief
             * Create .a library file using .o files under the specified object directory.
             */
            void build_lib(const string& obj_dir, const string& lib_target) const;

            /**
             * @brief
             * Link .o files into an executable
             */
            void build_app(const string& obj_dir, const string& target) const;

            /**
             * @brief
             * Link .o files into test executables
             */
            void build_tests(const string& obj_dir, const string& target) const;
        };
    } // namespace sdk
} // namespace ltd

#endif // _LTD_INCLUDE_COMPILER_HPP_