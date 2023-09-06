#include "pch.h"
#include "StringUtils.h"

std::string StringUtils::Trim(std::string& str)
{
    size_t first = str.find_first_not_of(" \t\n\r\f\v"); // Find the first non-whitespace character
    if (first == std::string::npos) {
        return ""; // If the string is all whitespace, return an empty string
    }
    size_t last = str.find_last_not_of(" \t\n\r\f\v"); // Find the last non-whitespace character
    return str.substr(first, (last - first + 1)); // Extract the trimmed substring
}

void StringUtils::ReplaceAll(std::string& str, const std::string& from, const std::string& to)
{
    size_t start = 0;
    while ((start = str.find(from, start)) != std::string::npos) {
        str.replace(start, from.length(), to);
        start += to.length(); // In case 'to' contains 'from', move the 'start' position
    }
}

std::string StringUtils::KeywordFormat(std::string str)
{
    uint64_t engineVer = VERSION_WHOLE;

    ReplaceAll(str, "{%eVer}", std::to_string(engineVer));

    return str;
}
