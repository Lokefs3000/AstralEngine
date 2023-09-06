#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "Data/Enum/TextureType.h"

__interface ITexture;

struct TextureRef
{
private:
	uint32_t m_Ref = UINT32_MAX;
public:
	TextureRef(uint32_t refId = UINT32_MAX) : m_Ref(refId) {};

	void Invalidate() { m_Ref = UINT32_MAX; };
	bool IsValid() { return m_Ref != UINT32_MAX; }
	uint32_t GetRefId() { return m_Ref; }
};

struct TextureManagerData
{
public:
	std::shared_ptr<TextureRef> Ref;
	std::shared_ptr<ITexture> Texture = NULL;
	TextureType Type = TextureType::Undefined;
	std::string TexturePath = "";
};