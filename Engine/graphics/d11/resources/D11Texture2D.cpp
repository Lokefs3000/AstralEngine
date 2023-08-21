#include "pch.h"
#include "D11Texture2D.h"

#include "graphics/d11/D11GraphicsContext.h"
#include "utils/D11Utils.h"

void D11Texture2D::Initialize(Texture2DOptions& options)
{
	D11GraphicsContext* ctx = (D11GraphicsContext*)options.Context.get();

	D3D11_TEXTURE2D_DESC texDesc{};
	texDesc.Width = options.Width;
	texDesc.Height = options.Height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	D3D11_SUBRESOURCE_DATA texData{};
	texData.pSysMem = options.Pixels;

	D11PerformCheck(ctx->GetDevice()->CreateTexture2D(&texDesc, &texData, m_Texture.GetAddressOf()), false, "Failed to create Texture2D");

	D3D11_SAMPLER_DESC samplerDesc{};
	
	switch (options.Filtering)
	{
	case 0:
		samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		break;
	case 1:
	default:
		samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
		break;
	}

	switch (options.Repeating)
	{
	case 0:
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		break;
	case 1:
	default:
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		break;
	}

	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxLOD = 1;
	
	D11PerformCheck(ctx->GetDevice()->CreateSamplerState(&samplerDesc, m_SamplerState.GetAddressOf()), false, "Failed to create SamplerState");

	D3D11_SHADER_RESOURCE_VIEW_DESC resViewDesc{};
	resViewDesc.Format = texDesc.Format;
	resViewDesc.ViewDimension = D3D10_1_SRV_DIMENSION_TEXTURE2D;
	resViewDesc.Texture2D.MipLevels = 1;

	D11PerformCheck(ctx->GetDevice()->CreateShaderResourceView(m_Texture.Get(), &resViewDesc, m_ResourceView.GetAddressOf()), false, "Failed to create SamplerState");

	m_IsInitialized = true;
}

void D11Texture2D::Shutdown()
{
}
