#include "pch.h"
#include "AssetManager.h"

#include <filesystem>
#include <fstream>
#include <sstream>

#include "Data/AssetManagerData.h"

typedef std::basic_ifstream<char16_t> u16ifstream;
typedef std::basic_stringstream<char16_t> u16stringstream;

void AssetManager::CreateGuids(AssetManagerData& data)
{
	if (data.IsPackaged) {

	}
	else {
		for (auto entry : std::filesystem::recursive_directory_iterator(data.SourceDir + u"assets\\"))
		{
			std::u16string path = entry.path().u16string();

			if (!std::filesystem::is_directory(path)) {
				if (std::filesystem::exists(path + u".ameta")) {
					std::ifstream ameta(entry.path().wstring() + L".ameta");
					std::stringstream metaS;
					metaS << ameta.rdbuf();
					ameta.close();

					xg::Guid guid(metaS.str());
					m_PathGuids.insert(std::make_pair(path.substr(8), guid));
					m_GuidPaths.insert(std::make_pair(guid, path));
				}
			}
		}
	}
}

void AssetManager::CreateGuidSource(xg::Guid& guid)
{
	std::wstring wstr(m_GuidPaths[guid].begin(), m_GuidPaths[guid].end());
	u16ifstream src(wstr, std::ios::binary);
	u16stringstream srcS;
	srcS << src.rdbuf();
	src.close();

	m_SourceGuids[guid] = srcS.str();
}

void AssetManager::Initialize(InitializableBasic* data)
{
	AssetManagerData& mdata = *(AssetManagerData*)data;

	CreateGuids(mdata);

	mR_SourceDir = mdata.SourceDir;
	mR_Packaged = mdata.IsPackaged;
}

void AssetManager::Shutdown()
{
}

xg::Guid AssetManager::GetAsset(std::u16string path)
{
	if (m_PathGuids.count(path))
		return m_PathGuids[path];

	return xg::Guid("00000000-0000-0000-0000-000000000000");
}

std::u16string AssetManager::GetSource(xg::Guid guid)
{
	if (m_SourceGuids.count(guid))
		return m_SourceGuids[guid];

	CreateGuidSource(guid);

	return m_SourceGuids[guid];
}
