#pragma once

#include "Variables.h"

#ifdef EXPOSE_RENDERER
#include <d3d11.h>
#include <wrl/client.h>
using namespace Microsoft::WRL;
class D11GraphicsContext;
#endif

#include "graphics/Renderer.h"

class D11Renderer : public IRenderer {
private:
#ifdef EXPOSE_RENDERER
	ComPtr<IDXGISwapChain> m_SwapChain;
	ComPtr<ID3D11Texture2D> m_BackBufferTexture;
	ComPtr<ID3D11RenderTargetView> m_BackBufferView;

	D11GraphicsContext* m_Context;

	void GetBackBuffers();
	void RefreshBuffers(uint32_t w, uint32_t h);
#endif
public:
	void InitializeRenderer(std::shared_ptr<Window> window, std::shared_ptr<IGraphicsContext> graphicsContext) override;
	void ShutdownRenderer() override;

	DEFINED_AS
};