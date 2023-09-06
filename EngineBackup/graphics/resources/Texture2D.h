#pragma once

#include "Variables.h"

#include "loader/Configurations.h"

#include <cstdint>
#include <memory>

__interface IGraphicsContext;

struct Texture2DOptions
{
public:
	IGraphicsContext* Context;

	uchar* Pixels = NULL;

	uint32_t Width;
	uint32_t Height;
	uint32_t Channels;

	uint32_t Filtering = 1;
	uint32_t Repeating;
};

__interface ITexture2D
{
public:
	virtual void EXPORT Initialize(Texture2DOptions options);
	virtual void EXPORT Shutdown();
};