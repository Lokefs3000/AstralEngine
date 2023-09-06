#pragma once

#include "Variables.h"

#include "graphics/managers/TextureManager.h"

#include "graphics/resources/Texture2D.h"
#include "graphics/resources/RenderTexture.h"

class ThreadPool;
__interface IAssetManager;

#include <vector>
#include <thread>
#include <mutex>

class D11Texture2D;
class D11RenderTexture;
class D11GraphicsContext;

class D11TextureManager : public ITextureManager {
private:
	std::vector<std::pair<std::string, TextureRef>> m_TextureReference{};
	std::vector<std::pair<TextureRef, std::shared_ptr<D11Texture2D>>> m_TextureList{};

	std::vector<std::pair<std::string, TextureRef>> m_RenderTextureReference{};
	std::vector<std::pair<TextureRef, std::shared_ptr<D11RenderTexture>>> m_RenderTextureList{};

	D11GraphicsContext* m_Context;
	bool m_isThreaded;

	std::shared_ptr<ThreadPool> m_resourcePool;

	std::shared_ptr<IAssetManager> m_assetManager;

	void EXPORT LoadTexture(std::shared_ptr<D11Texture2D> tex, TextureRef ref, std::string src, std::string meta);

	void EXPORT ReadjustReferences();
public:
	void EXPORT Initialize(std::shared_ptr<IGraphicsContext> Context, std::shared_ptr<IAssetManager> Assets, bool IsThreaded) override;
	void EXPORT Shutdown() override;

	TextureRef EXPORT GetTexture(std::string path) override;
	std::shared_ptr<ITexture2D> EXPORT GetTextureFromRef(TextureRef ref) override;

	TextureRef EXPORT GetRenderTexture(RenderTextureOptions options, std::string id) override;
	std::shared_ptr<IRenderTexture> EXPORT GetRenderTextureFromRef(TextureRef ref) override;

	void EXPORT LookForUnused() override;
	void EXPORT Clear() override;
};