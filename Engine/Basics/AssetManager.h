#pragma once

#include <string>

__interface IAssetManager
{
public:
	virtual void Initialize(std::string path);
	virtual void Shutdown();

	virtual std::string GetAsset(std::string path);
};