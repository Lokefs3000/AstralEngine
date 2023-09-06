#pragma once

#include <string>
#include <vector>
#include <map>
#include <variant>
#include <iostream>
#include <sstream>
#include <type_traits>

#include "utils/StringUtils.h"
#include "Variables.h"

struct ConfigObject
{
	std::string Name;
	std::map<std::string, std::variant<int, float, bool, std::string>> Values;
	std::vector<std::shared_ptr<ConfigObject>> Children;

	template<typename T>
	T GetValue(std::string key) {
		return std::get<T>(Values[key]);
	}

	std::shared_ptr<ConfigObject> GetChild(std::string name) {
		for (size_t i = 0; i < Children.size(); i++)
		{
			if (Children[i]->Name == name)
				return Children[i];
		}

		return NULL;
	}
};

class EXPORT Configurations {
private:
	std::pair<std::string, std::string> GetLineVariable(std::string line); //TODO: Error logs
	void HandleLineData(std::pair<std::string, std::string> data, std::shared_ptr<ConfigObject> current);
	void HandleObject(uint32_t& line, std::vector<std::string> lines, std::shared_ptr<ConfigObject> current, std::shared_ptr<ConfigObject> root);
	void HandleLine(uint32_t& line, std::vector<std::string> lines, std::shared_ptr<ConfigObject> root);
public:
	static std::shared_ptr<ConfigObject> LoadConfig(std::string data);

	std::shared_ptr<ConfigObject> LoadConfigLocal(std::string data);
};