#include "pch.h"
#include "AssetManagerRaw.h"

void AssetManagerRaw::Initialize(std::string src)
{
	m_Src = src + "\\assets\\";
}

std::string AssetManagerRaw::GetAsset(std::string assetName)
{
	if (std::filesystem::exists(m_Src + assetName) && !std::filesystem::is_directory(m_Src + assetName)) {
		std::ifstream file(m_Src + assetName, std::ios::binary);
		std::stringstream stream;
		stream << file.rdbuf();
		file.close();

		return stream.str();
	}

	return "";
}
