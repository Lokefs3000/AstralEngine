#pragma once

#include "Variables.h"

#include "graphics/resources/Texture2D.h"

#ifdef EXPOSE_TEXTURE2D
#include <d3d11.h>
#include <wrl/client.h>
using namespace Microsoft::WRL;
class D11GraphicsContext;
#endif

class D11Texture2D : public ITexture2D {
private:
#ifdef EXPOSE_TEXTURE2D
	ComPtr<ID3D11Texture2D> m_Texture;
	ComPtr<ID3D11SamplerState> m_SamplerState;
	ComPtr<ID3D11ShaderResourceView> m_ResourceView;
#endif

	bool m_IsInitialized = false;
public:
	void EXPORT Initialize(Texture2DOptions options) override;
	void EXPORT Shutdown() override;

	bool IsInitialized() { return m_IsInitialized; }

#ifdef EXPOSE_TEXTURE2D
	ID3D11Texture2D* GetTexture() { return m_Texture.Get(); }
	ID3D11SamplerState* GetSamplerState() { return m_SamplerState.Get(); }
	ID3D11ShaderResourceView* GetResourceView() { return m_ResourceView.Get(); }
#endif
};