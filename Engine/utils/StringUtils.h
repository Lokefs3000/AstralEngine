#pragma once

#include "Variables.h"
#include <string>

class EXPORT StringUtils {
public:
	static std::string Trim(std::string& str);
	static void ReplaceAll(std::string& str, const std::string& from, const std::string& to);
	static std::string KeywordFormat(std::string str);
};