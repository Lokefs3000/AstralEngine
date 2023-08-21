#pragma once

#include "Variables.h"

#include "graphics/managers/TextureManager.h"
#include "graphics/resources/Texture2D.h"

#include <vector>
#include <thread>
#include <mutex>

#ifdef EXPOSE_TEXTURE_MANAGER
class D11Texture2D;
class D11GraphicsContext;
#endif

class EXPORT D11TextureManager : public ITextureManager {
private:
#ifdef EXPOSE_TEXTURE_MANAGER
	std::vector<std::pair<std::string, TextureRef>> m_TextureReference;
	std::vector<std::pair<TextureRef, std::shared_ptr<D11Texture2D>>> m_TextureList;

	D11GraphicsContext* m_Context;
	bool m_isThreaded;

	uint32_t m_remainingTextures;
	bool m_killThread = false;
	std::thread m_loadingThread;
	std::mutex m_textureListMutex;
	std::condition_variable m_textureListCondition;

	void LoadTexture();

	void ReadjustReferences();
#endif
public:
	void Initialize(std::shared_ptr<IGraphicsContext> Context, bool IsThreaded) override;
	void Shutdown() override;

	TextureRef GetTexture(std::string path) override;
	std::shared_ptr<ITexture2D> GetTextureFromRef(TextureRef ref) override;

	void LookForUnused() override;
	void Clear() override;

};