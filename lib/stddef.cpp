#include "../inc/ltd/stddef.hpp"

namespace ltd
{
    string_list split(const string& text, const string& delimiter) 
    {
        string_list tokens;
        size_t start = 0, pos = 0, len = delimiter.length();
        string token;

        while ((pos = text.find(delimiter, start)) != string::npos) {
            token = text.substr(start, pos-start);
            tokens.push_back(token);
            start = pos + len;
        }
        
        token = text.substr(start, pos);
        tokens.push_back(token);

        return tokens;
    }
}