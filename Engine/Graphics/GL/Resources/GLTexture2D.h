#pragma once

#include "VarData.h"

#include "Basics/Texture.h"

class GLTexture2D : public ITexture {
private:
	uint32_t m_Texture;
public:
	void EXPORT Initialize(Texture2DData& data) override;
	void EXPORT Shutdown() override;

	void EXPORT Bind(uint32_t Slot) override;

#ifdef ALLOW_INCLUDE
	uint32_t EXPORT GetTextureId();
#endif
};