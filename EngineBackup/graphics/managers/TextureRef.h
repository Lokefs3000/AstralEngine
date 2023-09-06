#pragma once

#include "Variables.h"

#include <cstdint>
#include <memory>
#include <string>

struct EXPORT TextureRef_Struct
{
private:
	uint64_t m_RefId = 0;
	bool m_Invalid = false;
	bool m_IsRenderable = false;
	std::string m_Name = "";
public:
	TextureRef_Struct(uint64_t ref = 0, bool isRenderable = false, std::string name = "")
		: m_RefId(ref), m_Invalid(false), m_IsRenderable(isRenderable), m_Name(name) {};

	void Invalidate() { m_Invalid = true; }
	void ReconsiderRef(uint64_t ref = 0) { m_RefId = ref; }

	bool IsValid() { return !m_Invalid; }
	bool IsRenderable() { return m_IsRenderable; }
	uint64_t GetRefId() { return m_RefId; }
	std::string GetName() { return m_Name; }
};

typedef std::shared_ptr<TextureRef_Struct> TextureRef;
#define TextureRef_(...) std::make_shared<TextureRef_Struct>(__VA_ARGS__);