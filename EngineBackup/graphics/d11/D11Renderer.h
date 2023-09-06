#pragma once

#include "Variables.h"

#ifdef EXPOSE_RENDERER
#include <d3d11.h>
#include <wrl/client.h>
using namespace Microsoft::WRL;
class D11GraphicsContext;
class D11RenderTexture;
#endif

#include <utils/ApiUtils.h>
#include <graphics/managers/TextureRef.h>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics/Renderer.h"

class D11Renderer : public IRenderer {
private:
#ifdef EXPOSE_RENDERER
	ComPtr<IDXGISwapChain> m_SwapChain;
	ComPtr<ID3D11Texture2D> m_BackBufferTexture;
	ComPtr<ID3D11RenderTargetView> m_BackBufferView;

	ComPtr<ID3D11Buffer> m_UIBuffer;

	TextureRef m_PrimaryRenderRef;
	D11RenderTexture* m_PrimaryRender;

	D11GraphicsContext* m_Context;

	void GetBackBuffers();
	void RefreshBuffers(uint32_t w, uint32_t h);

	bool m_ShowRender = true;
#endif

	void ParseEntity(RendererRenderSceneData& data, std::shared_ptr<Scene> scene, std::shared_ptr<GameObject> objects, glm::mat4 pModel);
	RendererRenderSceneData ParseScene(std::shared_ptr<Scene> scene);
	void RenderScene(std::shared_ptr<Scene> scene);
public:
	void InitializeRenderer(std::shared_ptr<ConfigObject> config, std::shared_ptr<Window> window, std::shared_ptr<IGraphicsContext> graphicsContext, std::shared_ptr<ITextureManager> textures) override;
	void ShutdownRenderer() override;

	void ClearScreen() override;
	void RenderMain(std::vector<std::shared_ptr<Scene>> scenes) override;
	void PresentScreen() override;

	void ToggleShowRender() override;

#ifdef EXPOSE_RENDERER
	IDXGISwapChain* GetSwapChain() { return m_SwapChain.Get(); }
	ID3D11Texture2D* GetBackBuffer() { return m_BackBufferTexture.Get(); }
	ID3D11RenderTargetView* GetBackBufferView() { return m_BackBufferView.Get(); }
	D11RenderTexture* GetPrimaryTarget() { return m_PrimaryRender; }
#endif

	GraphicsAPI GetAPI() override;

	DEFINED_AS
};