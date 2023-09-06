#pragma once

#include <memory>
#include <string>

#include "Texture.h"
#include "AssetManager.h"
#include "Data/TextureManagerData.h"

__interface ITextureManager
{
public:
	virtual void Initialize(std::shared_ptr<IAssetManager> assetManager) = 0;
	virtual void Shutdown() = 0;

	virtual void ClearTextures(bool all = false) = 0;
	virtual void UpdateClearTimer(float dt) = 0;

	virtual std::shared_ptr<TextureRef> GetTexture2D(std::string path) = 0;
	virtual std::shared_ptr<ITexture> FromRef(std::shared_ptr<TextureRef> ref) = 0;
};