#pragma once

#include "Variables.h"

#include "AssetManager.h"

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>

class AssetManagerRaw : public IAssetManager {
private:
	std::string m_Src;
public:
	void EXPORT Initialize(std::string src) override;

	std::string EXPORT GetAsset(std::string assetName) override;
};