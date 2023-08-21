#pragma once

#include "Variables.h"

#include <cstdint>
#include <memory>

__interface IGraphicsContext;

struct Texture2DOptions
{
public:
	std::shared_ptr<IGraphicsContext> Context;

	uchar* Pixels;

	uint32_t Width;
	uint32_t Height;
	uint32_t Channels;

	uint32_t Filtering;
	uint32_t Repeating;
};

__interface ITexture2D
{
public:
	virtual void Initialize(Texture2DOptions& options);
	virtual void Shutdown();
};