#ifndef _LTD_IGNORE_FILTER_HPP_
#define _LTD_IGNORE_FILTER_HPP_

#include <regex>
#include <vector>

#include "../inc/ltd/stddef.hpp"

namespace ltd
{
	class ignore_filter
	{
	public:
		explicit ignore_filter(const string& ignore_file_path);

		bool is_ignored(const string& file_name) const;
		bool load_patterns();

	private:
		bool has_glob_tokens(const string& pattern) const;
		string trim(const string& value) const;
		string make_glob_regex(const string& pattern) const;

	private:
		string ignore_file_path;
		string_list exact_patterns;
		std::vector<std::regex> glob_patterns;
	};
} // namespace ltd


#endif // _LTD_IGNORE_FILTER_HPP_
