#include "pch.h"
#include "D11GraphicsContext.h"

#include "graphics/Window.h"

void D11GraphicsContext::InitializeContext(std::shared_ptr<Window> window)
{
	uint32_t deviceFlags = 0;
#ifdef _DEBUG
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	SDL_SysWMinfo windowInfo;
	SDL_GetWindowWMInfo(window->GetInternalWindow(), &windowInfo, SDL_SYSWM_CURRENT_VERSION);

	D11PerformCheck(D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		deviceFlags,
		NULL,
		0,
		D3D11_SDK_VERSION,
		m_Device.GetAddressOf(),
		NULL,
		m_ImmediateContext.GetAddressOf()), true, "Failed to create D3D11 device!");

	D11PerformCheck(m_Device->CreateDeferredContext(0, m_DeferredContext.GetAddressOf()), true, "Failed to create deferred context!");

	D11PerformCheck(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)m_Factory.GetAddressOf()), true, "Failed to create DXGI factory!");
}

void D11GraphicsContext::ShutdownContext()
{
}

GraphicsAPI D11GraphicsContext::GetAPI()
{
	return GraphicsAPI::Direct3D11;
}
