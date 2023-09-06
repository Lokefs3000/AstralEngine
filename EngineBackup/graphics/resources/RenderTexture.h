#pragma once

#include "Texture2D.h"

struct RenderTextureOptions : public Texture2DOptions {
public:
	bool HasDepthBuffer = false;
};

__interface IRenderTexture
{
public:
	virtual void Initialize(Texture2DOptions options);
	virtual void Shutdown();
};