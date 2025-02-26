#ifndef _LTD_INCLUDE_FMT_HPP_
#define _LTD_INCLUDE_FMT_HPP_

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

namespace ltd
{
    /**
     * @brief
     * Namespace fmt provides logging and formatting functionalities similar to 
     * stdio functions in C.
     */
    namespace fmt
    {
        enum class print_state {
            Next, Precision, Prepared
        };

        /**
         * @brief
         * struct printf_formatter provides internal mechanism in handling printf 
         * verbs.
         */
        struct printf_formatter {
            bool pound_flag = false;
            bool expect_width = false;
            bool expect_precision = false;
            bool precision_set = false;

            const char* read_adjustment_flag(std::ostream& out, const char* format);
            const char* read_sign_flag(std::ostream& out, const char* format);
            const char* read_pound_flag(std::ostream& out, const char* format);
            const char* read_fill_flag(std::ostream& out, const char* format);
            const char* read_width(std::ostream& out, const char* format);
            const char* read_precision(std::ostream& out, const char* format);
            const char* read_length(std::ostream& out, const char* format);
            const char* read_specifier(std::ostream& out, const char* format);
        };

        template<typename T>
        void osprintf(std::ostream& out, T arg)
        {
            out << arg;
        }

        /**
         * @brief
         * Default function for osprintf.
         */
        void osprintf(print_state state, std::ostream& out, const char* format);

        /**
         * @brief
         * Function template for handling printf like functionality.
         */
        template<typename T, typename... Args>
        void osprintf(print_state state, std::ostream& out, const char* format, T value, Args ...args)
        {
            switch (state) {
            case print_state::Precision:
                break;
            case print_state::Prepared:
                break;
            case print_state::Next:
                while (*format != 0) {
                    if (*format == '%') {
                        format++;

                        // Reset formatting
                        out.width(0);
                        out.precision(0);
                        out.fill(' ');

                        printf_formatter formatter;

                        // Adjusment flag
                        if ((format = formatter.read_adjustment_flag(out, format)) == nullptr)
                            return;

                        // Sign flag
                        if ((format = formatter.read_sign_flag(out, format)) == nullptr)
                            return;

                        // TODO: There is no implementation for print space
                        // if there's no sign printed.

                        // Pound flag
                        if ((format = formatter.read_pound_flag(out, format)) == nullptr)
                            return;

                        // Fill with 0's flag
                        if ((format = formatter.read_fill_flag(out, format)) == nullptr)
                            return;

                        // Set print width
                        if ((format = formatter.read_width(out, format)) == nullptr)
                            return;

                        // Set precision
                        if ((format = formatter.read_precision(out, format)) == nullptr)
                            return;

                        // Length is ignored
                        if ((format = formatter.read_length(out, format)) == nullptr)
                            return;

                        // Specifier
                        if ((format = formatter.read_specifier(out, format)) == nullptr)
                            return;

                        if (formatter.expect_width) {
                            if (formatter.expect_precision)
                                return osprintf(print_state::Precision, out, format, args...);
                            else
                                return osprintf(print_state::Prepared, out, format, args...);
                        } else if (formatter.expect_precision) {
                            return osprintf(print_state::Prepared, out, format, args...);
                        }

                        out << value;

                        return osprintf(print_state::Next, out, format, args...);
                    } else {
                        out << *format++;
                    }
                }
            default:
                break;
            }
        }

        /**
         * @brief
         * Function template for printf.
         */
        template<typename... Args>
        void printf(const char* format, Args... args)
        {
            osprintf(print_state::Next, std::cout, format, args...);
        }

        /**
         * @brief
         * Default printf function to print out single object without format
        */
        template<typename T>
        void printf(T arg)
        {
            osprintf(std::cout, arg);
        }

        /**
         * @brief
         * Function template for printf with carriage return.
         */
        template<typename... Args>
        void println(const char* format, Args... args)
        {
            osprintf(print_state::Next, std::cout, format, args...);
            std::cout << std::endl;
        }

        /**
         * @brief
         * Default println function to print out single object without format
        */
        template<typename T>
        void println(T arg)
        {
            osprintf(std::cout, arg);
            std::cout << std::endl;
        }

        /**
         * @brief
         * Function template for sprintf.
         */
        template<typename... Args>
        std::string sprintf(const char* format, Args... args)
        {
            std::ostringstream sstream;
            osprintf(print_state::Next, sstream, format, args...);

            return sstream.str();
        }

        /**
         * @brief
         * Function template for sprintf with single object and no format.
         */
        template<typename T>
        std::string sprintf(T arg)
        {
            std::ostringstream sstream;
            osprintf(sstream, arg);

            return sstream.str();
        }

        /**
         * @brief
         * Function template for printing to string with carriage return.
         */
        template<typename... Args>
        std::string sprintln(const char* format, Args... args)
        {
            std::ostringstream sstream;
            osprintf(print_state::Next, sstream, format, args...);
            sstream << std::endl;

            return sstream.str();
        }

        /**
         * @brief
         * Function template for sprintln with single object and no format.
         */
        template<typename T>
        std::string sprintln(T arg)
        {
            std::ostringstream sstream;
            osprintf(sstream, arg);
            sstream << std::endl;

            return sstream.str();
        }
    } // namespace fmt
} // namespace ltd

#endif // _LTD_INCLUDE_FMT_HPP_