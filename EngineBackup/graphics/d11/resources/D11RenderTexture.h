#pragma once

#include "Variables.h"

#include "graphics/resources/RenderTexture.h"

#ifdef EXPOSE_TEXTURE2D
#include <d3d11.h>
#include <wrl/client.h>
using namespace Microsoft::WRL;
class D11GraphicsContext;
#endif

#include <glm/glm.hpp>

class D11RenderTexture : public IRenderTexture {
private:
#ifdef EXPOSE_TEXTURE2D
	ComPtr<ID3D11Texture2D> m_Texture;
	ComPtr<ID3D11RenderTargetView> m_RenderTarget;
	ComPtr<ID3D11SamplerState> m_SamplerState;
	ComPtr<ID3D11ShaderResourceView> m_ResourceView;

	D11GraphicsContext* m_Context;
	Texture2DOptions m_Options;
#endif

	bool m_IsInitialized = false;

	void LoadTexture();
public:
	void Initialize(Texture2DOptions options) override;
	void Shutdown() override;

	void Resize(glm::ivec2 size);

	bool IsInitialized() { return m_IsInitialized; }

#ifdef EXPOSE_TEXTURE2D
	ID3D11Texture2D* GetTexture() { return m_Texture.Get(); }
	ID3D11RenderTargetView* GetRenderTarget() { return m_RenderTarget.Get(); }
	ID3D11SamplerState* GetSamplerState() { return m_SamplerState.Get(); }
	ID3D11ShaderResourceView* GetResourceView() { return m_ResourceView.Get(); }
#endif
};