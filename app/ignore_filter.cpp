#include "ignore_filter.hpp"

#include <fstream>

#include "../inc/ltd/log.hpp"
#include "../inc/ltd/stdfs.hpp"

namespace ltd
{
	ignore_filter::ignore_filter(const string& ignore_file_path)
		: ignore_file_path(ignore_file_path)
	{
	}

	bool ignore_filter::is_ignored(const string& file_name) const
	{
		if (file_name.empty() || !is_loaded) {
			return false;
		}

		auto base_name = fs::path(file_name).filename().generic_string();

		for (const auto& pattern : exact_patterns) {
			if (pattern == file_name || pattern == base_name) {
				return true;
			}
		}

		for (const auto& pattern : glob_patterns) {
			if (std::regex_match(file_name, pattern) || std::regex_match(base_name, pattern)) {
				return true;
			}
		}

		return false;
	}

	bool ignore_filter::load_patterns()
	{
		exact_patterns.clear();
		glob_patterns.clear();

        if (!fs::is_regular_file(ignore_file_path)) {
            return false;
        }

		std::ifstream file(ignore_file_path);
		if (!file.is_open()) {
			return false;
		}

		string line;
		while (std::getline(file, line)) {
			auto pattern = trim(line);
			if (pattern.empty() || pattern.at(0) == '#') {
				continue;
			}

			if (has_glob_tokens(pattern)) {
                string rex_pattern = make_glob_regex(pattern);
				glob_patterns.emplace_back(rex_pattern);
			} else {
				exact_patterns.push_back(pattern);
			}
		}

		is_loaded = true;
		return true;
	}

	bool ignore_filter::has_glob_tokens(const string& pattern) const
	{
		return pattern.find('*') != string::npos || pattern.find('?') != string::npos;
	}

	string ignore_filter::trim(const string& value) const
	{
		auto begin = value.find_first_not_of(" \t\r\n");
		if (begin == string::npos) {
			return "";
		}

		auto end = value.find_last_not_of(" \t\r\n");
		return value.substr(begin, end - begin + 1);
	}

	string ignore_filter::make_glob_regex(const string& pattern) const
	{
		string regex_pattern = "^";

		for (char ch : pattern) {
			switch (ch) {
				case '*':
					regex_pattern += ".*";
					break;
				case '?':
					regex_pattern += ".";
					break;
				case '.':
				case '^':
				case '$':
				case '|':
				case '(':
				case ')':
				case '[':
				case ']':
				case '{':
				case '}':
				case '+':
				case '\\':
					regex_pattern += '\\';
					regex_pattern += ch;
					break;
				default:
					regex_pattern += ch;
					break;
			}
		}

		regex_pattern += "$";
		return regex_pattern;
	}
} // namespace ltd
