#include "pch.h"
#include "RawAssetManager.h"

#include <fstream>
#include <sstream>

void RawAssetManager::Initialize(std::string path)
{
    m_ProjectDir = path;
    m_AssetDir = path + "assets\\";
}

void RawAssetManager::Shutdown()
{
}

std::string RawAssetManager::GetAsset(std::string path)
{
    std::ifstream file(m_AssetDir + path, std::ios::binary);
    std::stringstream stream;
    stream << file.rdbuf();
    file.close();

    return stream.str();
}
