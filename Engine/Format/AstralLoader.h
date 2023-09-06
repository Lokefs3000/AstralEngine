#pragma once

#include <memory>
#include <string>
#include <vector>
#include <variant>
#include "VarData.h"

struct AstralObject;

static class EXPORT AstralLoader {
private:
	static std::pair<std::string, std::string> ParseLine(std::string line);
	static void HandleObject(std::vector<std::string>& lines, size_t& idx, std::string id, std::shared_ptr<AstralObject> parent, bool& isInArray, std::vector<std::variant<int, float, bool, std::string>>& listArray, std::string& listArrayName);
	static bool HandleLineData(std::vector<std::string>& lines, size_t& idx, std::shared_ptr<AstralObject> parent, bool& isInArray, std::vector<std::variant<int, float, bool, std::string>>& listArray, std::string& listArrayName);
public:
	static std::shared_ptr<AstralObject> LoadAstral(std::string src);
};