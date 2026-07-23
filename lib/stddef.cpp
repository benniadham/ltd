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

    string join(const string_list& tokens, const string& delimiter) 
    {
        string result;
        for (size_t i = 0; i < tokens.size(); ++i) {
            result += tokens[i];
            if (i < tokens.size() - 1) {
                result += delimiter;
            }
        }
        return result;
    }
    
    bool starts_with(const string& text, const string& prefix)
    {
        return text.rfind(prefix, 0) == 0;
    }

    bool ends_with(const string& text, const string& suffix)
    {
        if (text.length() < suffix.length()) return false;
        return text.compare(text.length() - suffix.length(), suffix.length(), suffix) == 0;
    }
}