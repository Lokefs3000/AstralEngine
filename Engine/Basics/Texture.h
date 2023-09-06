#pragma once

#include <cstdint>

struct Texture2DData;

__interface ITexture
{
public:
	virtual void Initialize(Texture2DData& data);
	virtual void Shutdown();

	virtual void Bind(uint32_t Slot);
};