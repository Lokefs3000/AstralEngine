#pragma once

#include <vector>

#include "VarData.h"

#include "Basics/TextureManager.h"

struct TextureManagerData;

class GLTextureManager : public ITextureManager {
private:
	std::vector<TextureManagerData*> m_Data;
	std::shared_ptr<IAssetManager> m_Assets;

	float m_ClearTimer = 0.0f;
public:
	void EXPORT Initialize(std::shared_ptr<IAssetManager> assetManager) override;
	void EXPORT Shutdown() override;

	void EXPORT ClearTextures(bool all) override;
	void EXPORT UpdateClearTimer(float dt) override;

	std::shared_ptr<TextureRef> EXPORT GetTexture2D(std::string path) override;
	std::shared_ptr<ITexture> EXPORT FromRef(std::shared_ptr<TextureRef> ref) override;
};