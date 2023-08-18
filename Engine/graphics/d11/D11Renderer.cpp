#include "pch.h"
#include "D11Renderer.h"

#include "graphics/Window.h"
#include "graphics/d11/D11GraphicsContext.h"

void D11Renderer::GetBackBuffers()
{
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_BackBufferTexture.Get());

	D3D11_RENDER_TARGET_VIEW_DESC backViewDesc{};
	backViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	backViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	
	D11PerformCheck(m_Context->GetDevice()->CreateRenderTargetView(m_BackBufferTexture.Get(), &backViewDesc, m_BackBufferView.GetAddressOf()), true, "SwapChain resizing failed!");
}

void D11Renderer::RefreshBuffers(uint32_t w, uint32_t h)
{
	m_BackBufferTexture.Reset();
	m_BackBufferView.Reset();

	m_SwapChain->ResizeBuffers(2, w, h, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	GetBackBuffers();
}

void D11Renderer::InitializeRenderer(std::shared_ptr<Window> window, std::shared_ptr<IGraphicsContext> graphicsContext)
{
	D11GraphicsContext* ctx = (D11GraphicsContext*)graphicsContext.get();
	m_Context = ctx;

	int wW, wH;
	SDL_GetWindowSizeInPixels(window->GetInternalWindow(), &wW, &wH);

	SDL_SysWMinfo windowInfo;
	SDL_GetWindowWMInfo(window->GetInternalWindow(), &windowInfo, SDL_SYSWM_CURRENT_VERSION);

	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferDesc.Width = wW;
	swapChainDesc.BufferDesc.Height = wH;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferCount = 2; //Double buffer no question!
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = windowInfo.info.win.window;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

	ctx->GetFactory()->CreateSwapChain(ctx->GetDevice(), &swapChainDesc, m_SwapChain.GetAddressOf());

	GetBackBuffers();
}

void D11Renderer::ShutdownRenderer()
{
	m_Context = NULL;
}
