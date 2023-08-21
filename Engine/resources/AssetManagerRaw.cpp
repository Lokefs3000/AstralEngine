#include "pch.h"
#include "AssetManagerRaw.h"

void AssetManagerRaw::Initialize(std::string src)
{
	m_Src = src;

	ReloadAssets();
}

void AssetManagerRaw::ReloadAssets()
{
	m_Resources.clear();

	for (auto entry : std::filesystem::recursive_directory_iterator())
	{
		if (!std::filesystem::is_directory(entry.path())) {
			m_Resources.push_back(entry.path().string().substr(m_Src.size()));
		}
	}
}

std::string AssetManagerRaw::GetAsset(std::string assetName)
{
    return std::string();
}
