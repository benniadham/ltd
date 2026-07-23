#ifndef _LTD_INCLUDE_STDFS_HPP_
#define _LTD_INCLUDE_STDFS_HPP_

#include <filesystem>

#include "stddef.hpp"

namespace ltd
{
    namespace fs = std::filesystem;
    using file_ts = fs::file_time_type;

    /**
     * @brief 
     * Collects all files with the specified extension from the given directory and 
     * stores them in the provided string_list. 
     * 
     * @param dir   The directory to search for files.
     * @param ext   The file extension to filter by. 
     *              This can be a single extension (e.g., ".cpp") or 
     *              a string containing multiple extensions (e.g., ".cpp;.cc;.cxx").
     * @param files The list to store the collected file paths.
     * 
     * @return true if the operation was successful, false otherwise.
     */
    bool collect_files(const string& dir, const string& ext, string_list& files);

    /**
     * @brief 
     * Converts a file timestamp to a human-readable string representation.
     * 
     * @param timestamp The file timestamp to convert.
     * @return A string representing the timestamp in the format: "Day, DD Mon YYYY HH:MM:SS".
     */
    string string_ts(const file_ts& timestamp);
} // namespace ltd
#endif // _LTD_INCLUDE_STDFS_HPP_