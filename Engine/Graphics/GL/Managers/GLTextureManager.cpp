#include "pch.h"
#include "GLTextureManager.h"

#include "Basics/AssetManager.h"
#include "Basics/Texture.h"

#include "Data/TextureManagerData.h"
#include "Data/TextureData.h"

#include "Graphics/GL/Resources/GLTexture2D.h"

#include "Debug/Logger.h"

#include <stb_image.h>

void GLTextureManager::ClearTextures(bool all)
{
	uint32_t clearCount = 0;

	if (all) {
		for (size_t i = 0; i < m_Data.size(); i++) {
			m_Data[i]->Ref->Invalidate();
			delete m_Data[i];
			clearCount++;
		}
	}
	else {
		for (size_t i = 0; i < m_Data.size(); i++) {
			if (m_Data[i]->Ref.use_count() <= 1) {
				m_Data[i]->Ref->Invalidate();
				delete m_Data[i];
				m_Data.erase(m_Data.begin() + i);
				clearCount++;
			}
		}
	}

	Debug("Cleared: ", clearCount, " textures from memory!");
}

void GLTextureManager::UpdateClearTimer(float dt)
{
	m_ClearTimer -= dt;
	if (m_ClearTimer < 0.0f) {
		m_ClearTimer = 10.0f;

		ClearTextures(false);
	}
}

std::shared_ptr<TextureRef> GLTextureManager::GetTexture2D(std::string path)
{
	for (size_t i = 0; i < m_Data.size(); i++)
		if (m_Data[i]->Ref->IsValid() && m_Data[i]->TexturePath == path && m_Data[i]->Type == TextureType::Texture2D)
			return m_Data[i]->Ref;

	std::string assetSrc = m_Assets->GetAsset(path);
	if (!assetSrc.empty()) {
		Texture2DData texData{};
		int w, h, ch;
		texData.Pixels = stbi_load_from_memory((stbi_uc*)assetSrc.c_str(), assetSrc.size(), &w, &h, &ch, 0);
		texData.Width = w;
		texData.Height = h;
		texData.Channels = (TextureChannels)ch;
		texData.FilteringFar = TextureFiltering::Point;
		texData.FilteringNear = TextureFiltering::Point;
		texData.Repeating = TextureRepeating::Clamp;
		texData.Mipmaps = 1;

		if (texData.Pixels == NULL) {
			Warning("Failed to load texture: ", path, ", failure reason: ", stbi_failure_reason());
			stbi_image_free(texData.Pixels);
			return NULL;
		}

		TextureManagerData* data = new TextureManagerData();
		data->TexturePath = path;
		data->Ref = std::make_shared<TextureRef>(m_Data.size());
		data->Type = TextureType::Texture2D;

		data->Texture = std::make_shared<GLTexture2D>();
		data->Texture->Initialize(texData);

		stbi_image_free(texData.Pixels);

		m_Data.push_back(data);
		return data->Ref;
	}

	Warning("Failed to find texture: ", path);

	return NULL;
}

std::shared_ptr<ITexture> GLTextureManager::FromRef(std::shared_ptr<TextureRef> ref)
{
	if (ref.get() != NULL && ref->IsValid() && ref->GetRefId() < m_Data.size())
		return m_Data[ref->GetRefId()]->Texture;
	Warning("Failed to find texture for reference. Reference id: ", ref->GetRefId(), ", Is valid: ", ref->IsValid());
	return NULL;
}

void GLTextureManager::Initialize(std::shared_ptr<IAssetManager> assetManager)
{
	m_Assets = assetManager;
}

void GLTextureManager::Shutdown()
{
	ClearTextures(true);
}
