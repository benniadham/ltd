#include "../inc/ltd/stdfs.hpp"

#include <chrono>

namespace ltd
{
    bool collect_files(const string& dir, const string& ext, string_list& files)
    {
        for (const auto& entry : fs::directory_iterator(dir)) {
            if (entry.is_regular_file()==false)
                continue;
            if (ext.find_first_of(entry.path().extension().string()) != string::npos) {
                files.push_back(entry.path().string());
            }
        }
        return true;
    }

    template <typename TP>
    std::time_t to_time_t(TP tp)
    {
        using namespace std::chrono;
        auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
                + system_clock::now());
        return system_clock::to_time_t(sctp);
    }

    string string_ts(const file_ts& timestamp)
    {
        std::time_t tt = to_time_t(timestamp);
        std::tm *gmt = std::gmtime(&tt);
        std::stringstream buffer;

        buffer << std::put_time(gmt, "\t%a, %d %b %Y %H:%M:%S");

        return buffer.str();
    }
} // namespace ltd 