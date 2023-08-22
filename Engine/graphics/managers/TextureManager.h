#pragma once

#include "Variables.h"

#include "TextureRef.h"

#include "graphics/resources/Texture2D.h"

#include <string>

__interface IGraphicsContext;
__interface IAssetManager;

__interface ITextureManager {
public:
	void EXPORT Initialize(std::shared_ptr<IGraphicsContext> Context, std::shared_ptr<IAssetManager> Assets, bool IsThreaded);
	void EXPORT Shutdown();

	TextureRef GetTexture(std::string path);
	std::shared_ptr<ITexture2D> GetTextureFromRef(TextureRef ref);

	void EXPORT LookForUnused();
	void EXPORT Clear();
};