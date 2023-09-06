#pragma once

#include "Variables.h"

#include <string>

__interface EXPORT IAssetManager
{
public:
	void Initialize(std::string src);

	std::string GetAsset(std::string assetName);
};