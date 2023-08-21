#pragma once

#include "Variables.h"

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>

class EXPORT AssetManagerRaw {
private:
	std::vector<std::string> m_Resources;

	std::string m_Src;
public:
	void Initialize(std::string src);

	void ReloadAssets();

	std::string GetAsset(std::string assetName);
};