#include "pch.h"
#include "DebugRenderer.h"

#include "Data/DebugRendererData.h"

#include "RendererCore.h"
#include "ShaderManager.h"

void DebugRenderer::CreateShaders(DebugRendererData& data)
{
	//data.Shaders->GetShader()
}

void DebugRenderer::Initialize(InitializableBasic* data)
{
	DebugRendererData& mdata = *(DebugRendererData*)data;

	CreateShaders(mdata);
}

void DebugRenderer::Shutdown()
{
}
