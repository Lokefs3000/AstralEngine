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
	/*ID3D11Debug* debug = NULL;
	D11PerformCheck(m_Device->QueryInterface(__uuidof(ID3D11Debug), (void**)&debug), false, "Failed to get ID3D11Debug.");
	if (debug != NULL) {
		debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		debug->Release();
	}*/
}

void D11GraphicsContext::ObjectReport()
{
	IDXGIDebug1* debugDX = NULL;
	D11PerformCheck(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debugDX)), false, "Failed to get IDXGIDebug1.");
	if (debugDX != NULL) {
		debugDX->ReportLiveObjects({ 0xe48ae283, 0xda80, 0x490b, 0x87, 0xe6, 0x43, 0xe9, 0xa9, 0xcf, 0xda, 0x8 }, DXGI_DEBUG_RLO_ALL);
		debugDX->Release();
	}
}

GraphicsAPI D11GraphicsContext::GetAPI()
{
	return GraphicsAPI::Direct3D11;
}
