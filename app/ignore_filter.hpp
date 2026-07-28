#ifndef _LTD_IGNORE_FILTER_HPP_
#define _LTD_IGNORE_FILTER_HPP_

#include <regex>
#include <vector>

#include "../inc/ltd/stddef.hpp"

namespace ltd
{
	/**
	 * @brief 
	 * The ignore_filter class is responsible for loading and applying ignore 
	 * patterns from a specified .ignore file.
	 * 
	 * It provides functionality to check if a given file name matches any of 
	 * the loaded ignore patterns. 
	 */
	class ignore_filter
	{
	public:
		/**
		 * @brief Construct a new ignore_filter object
		 * 
		 * @param ignore_file_path The path to the .ignore file.
		 */
		explicit ignore_filter(const string& ignore_file_path);

		/**
		 * @brief Check if a given file name is ignored based on the loaded patterns.
		 * 
		 * @param file_name The name of the file to check.
		 * @return true if the file name matches any ignore pattern, false otherwise.
		 */
		bool is_ignored(const string& file_name) const;

		/**
		 * @brief Load ignore patterns from the specified .ignore file.
		 * 
		 * @return true if the patterns were successfully loaded, false otherwise.
		 */
		bool load_patterns();

	private:
		bool has_glob_tokens(const string& pattern) const;
		string trim(const string& value) const;
		string make_glob_regex(const string& pattern) const;

	private:
		string ignore_file_path;
		string_list exact_patterns;
		std::vector<std::regex> glob_patterns;
		bool is_loaded = false;
	};
} // namespace ltd


#endif // _LTD_IGNORE_FILTER_HPP_
