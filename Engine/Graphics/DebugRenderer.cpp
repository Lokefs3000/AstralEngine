#include "pch.h"
#include "DebugRenderer.h"

#include "Data/DebugRendererData.h"

#include "RendererCore.h"
#include "ShaderManager.h"

#include "Assets/AssetManager.h"

void DebugRenderer::CreateShaders(DebugRendererData& data)
{
	data.Shaders->GetShader(xg::Guid("2f3c5155-ad3b-4764-b934-72587a4e0d9f"), xg::Guid("8f4196e6-2299-4fb5-b2e4-d5ced06e093f"));
}

void DebugRenderer::Initialize(InitializableBasic* data)
{
	DebugRendererData& mdata = *(DebugRendererData*)data;

	CreateShaders(mdata);

	mR_AssetManager = mdata.AssetManager;
}

void DebugRenderer::Shutdown()
{
}

void DebugRenderer::Render()
{
}
