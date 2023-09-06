#include "pch.h"
#include "D11RenderTexture.h"

#include "graphics/d11/D11GraphicsContext.h"
#include "utils/D11Utils.h"

void D11RenderTexture::LoadTexture()
{
	D3D11_TEXTURE2D_DESC texDesc{};
	texDesc.Width = m_Options.Width;
	texDesc.Height = m_Options.Height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	D3D11_SUBRESOURCE_DATA texData{};
	texData.pSysMem = m_Options.Pixels;
	texData.SysMemPitch = 4 * sizeof(uchar) * m_Options.Width;

	D11PerformCheck(m_Context->GetDevice()->CreateTexture2D(&texDesc, m_Options.Pixels == NULL ? NULL : &texData, m_Texture.ReleaseAndGetAddressOf()), false, "Failed to create Texture2D");

	D3D11_RENDER_TARGET_VIEW_DESC renderViewDesc{};
	renderViewDesc.Format = texDesc.Format;
	renderViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	D11PerformCheck(m_Context->GetDevice()->CreateRenderTargetView(m_Texture.Get(), &renderViewDesc, m_RenderTarget.ReleaseAndGetAddressOf()), false, "Failed to create RenderTarget");

	D3D11_SAMPLER_DESC samplerDesc{};

	switch (m_Options.Filtering)
	{
	case 0:
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		break;
	case 1:
	default:
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		break;
	}

	switch (m_Options.Repeating)
	{
	case 0:
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		break;
	case 1:
	default:
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		break;
	}

	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxLOD = 1;

	D11PerformCheck(m_Context->GetDevice()->CreateSamplerState(&samplerDesc, m_SamplerState.ReleaseAndGetAddressOf()), false, "Failed to create SamplerState");

	D3D11_SHADER_RESOURCE_VIEW_DESC resViewDesc{};
	resViewDesc.Format = texDesc.Format;
	resViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resViewDesc.Texture2D.MipLevels = 1;

	D11PerformCheck(m_Context->GetDevice()->CreateShaderResourceView(m_Texture.Get(), &resViewDesc, m_ResourceView.ReleaseAndGetAddressOf()), false, "Failed to create SamplerState");
}

void D11RenderTexture::Initialize(Texture2DOptions options)
{
	D11GraphicsContext* ctx = (D11GraphicsContext*)options.Context;
	m_IsInitialized = true;
	m_Context = ctx;
	m_Options = options;

	LoadTexture();
}

void D11RenderTexture::Shutdown()
{
}

void D11RenderTexture::Resize(glm::ivec2 size)
{
	if (!m_Context || !m_IsInitialized)
		return;

	m_Options.Pixels = NULL;
	m_Options.Width = size.x;
	m_Options.Height = size.y;

	LoadTexture();
}
