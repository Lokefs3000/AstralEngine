#pragma once

#include "Variables.h"

#include "TextureRef.h"

#include "graphics/resources/Texture2D.h"

#include <string>

__interface IGraphicsContext;

__interface EXPORT ITextureManager {
public:
	void Initialize(std::shared_ptr<IGraphicsContext> Context, bool IsThreaded);
	void Shutdown();

	TextureRef GetTexture(std::string path);
	std::shared_ptr<ITexture2D> GetTextureFromRef(TextureRef ref);

	void LookForUnused();
	void Clear();
};