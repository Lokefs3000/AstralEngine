#pragma once

#include "VarData.h"

#include "Basics/AssetManager.h"

class RawAssetManager : public IAssetManager {
private:
	std::string m_ProjectDir;
	std::string m_AssetDir;
public:
	void EXPORT Initialize(std::string path) override;

	void EXPORT Shutdown() override;

	std::string EXPORT GetAsset(std::string path) override;
};