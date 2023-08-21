#pragma once

#include "Variables.h"
#include "graphics/GraphicsContext.h"

#ifdef EXPOSE_CONTEXT
#include <SDL3/SDL_syswm.h>
#include <wrl/client.h>
#include <d3d11.h>
#include "utils/D11Utils.h"
using namespace Microsoft::WRL;
#endif

#include <utils/ApiUtils.h>

class D11GraphicsContext : public IGraphicsContext {
private:
#ifdef EXPOSE_CONTEXT
	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3D11DeviceContext> m_ImmediateContext;
	ComPtr<ID3D11DeviceContext> m_DeferredContext;
	ComPtr<IDXGIFactory> m_Factory;
#endif
public:
	void InitializeContext(std::shared_ptr<Window> window) override;
	void ShutdownContext() override;

#ifdef EXPOSE_CONTEXT
	ID3D11Device* GetDevice() { return m_Device.Get(); };
	ID3D11DeviceContext* GetImmediate() { return m_ImmediateContext.Get(); };
	ID3D11DeviceContext* GetDeferred() { return m_DeferredContext.Get(); };
	IDXGIFactory* GetFactory() { return m_Factory.Get(); };
#endif

	GraphicsAPI GetAPI() override;

	DEFINED_AS
};