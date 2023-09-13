#pragma once

#include "VarData.h"

#include <map>
#include <string>

#include <Guid.hpp>

#include "Basics/Initializable.h"

struct AssetManagerData;

class AssetManager : public IInitializable {
private:
	std::map<std::u16string, std::string> m_PathGuids;
	std::map<std::string, std::u16string> m_GuidPaths;
	std::map<std::string, std::u16string> m_SourceGuids;

	std::u16string mR_SourceDir;
	bool mR_Packaged;

	void CreateGuids(AssetManagerData& data);
	void CreateGuidSource(xg::Guid& guid);
public:
	void EXPORT Initialize(InitializableBasic* data) override;
	void EXPORT Shutdown() override;

	xg::Guid EXPORT GetAsset(std::u16string path);
	std::u16string EXPORT GetSource(xg::Guid guid);
};