#pragma once

#include <cstdint>
#include <memory>

struct TextureRef_Struct
{
private:
	uint64_t m_RefId = 0;
	bool m_Invalid = false;
public:
	TextureRef_Struct(uint64_t ref = 0)
		: m_RefId(ref), m_Invalid(false) {};

	void Invalidate() { m_Invalid = true; }
	void ReconsiderRef(uint64_t ref = 0) { m_RefId = ref; }

	bool IsValid() { return !m_Invalid; }
	uint64_t GetRefId() { return m_RefId; }
};

typedef std::shared_ptr<TextureRef_Struct> TextureRef;
#define TextureRef_(ref) std::make_shared<TextureRef_Struct>(ref);