#pragma once

#include <cstdint>

#include "ContextBase.h"

enum class TextureFiltering {
	Point,
	Bilinear
};

enum class TextureRepeating {
	Clamp,
	Repeat
};

enum class TextureChannels {
	None,
	Red,
	Grayscale,
	RGB,
	RGBA
};

struct TextureData : public ContextBase
{
public:
	uint32_t Width;
	uint32_t Height;

	TextureFiltering FilteringFar;
	TextureFiltering FilteringNear;

	TextureRepeating Repeating;

	TextureChannels Channels;
};

struct Texture2DData : public TextureData
{
public:
	void* Pixels;

	uint32_t Mipmaps;
};