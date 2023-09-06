#pragma once

#include "Variables.h"

#include "TextureRef.h"

#include "graphics/resources/Texture2D.h"
#include "graphics/resources/RenderTexture.h"

#include <string>

__interface IGraphicsContext;
__interface IAssetManager;

__interface ITextureManager {
public:
	void Initialize(std::shared_ptr<IGraphicsContext> Context, std::shared_ptr<IAssetManager> Assets, bool IsThreaded);
	void Shutdown();

	TextureRef GetTexture(std::string path);
	std::shared_ptr<ITexture2D> GetTextureFromRef(TextureRef ref);

	TextureRef GetRenderTexture(RenderTextureOptions options, std::string id);
	std::shared_ptr<IRenderTexture> GetRenderTextureFromRef(TextureRef ref);

	void LookForUnused();
	void Clear();
};