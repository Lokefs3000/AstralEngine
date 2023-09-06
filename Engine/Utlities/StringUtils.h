#pragma once

#include "VarData.h"
#include <string>
#include <vector>

class EXPORT StringUtils {
public:
	static std::string Trim(std::string& str);
	static void ReplaceAll(std::string& str, const std::string& from, const std::string& to);
	static std::string KeywordFormat(std::string str);
	static std::vector<std::string> Split(const std::string& input, char delimiter);
};