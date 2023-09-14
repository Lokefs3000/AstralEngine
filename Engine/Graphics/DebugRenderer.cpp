#include "pch.h"
#include "DebugRenderer.h"

#include "Data/DebugRendererData.h"
#include "Data/BufferData.h"

#include "RendererCore.h"
#include "ShaderManager.h"
#include "BufferManager.h"
#include "Buffer.h"
#include "Shader.h"

#include "Assets/AssetManager.h"

void DebugRenderer::CreateShaders(DebugRendererData& data)
{
	m_DebugScreenShader = data.Shaders->GetShader(xg::Guid("2f3c5155-ad3b-4764-b934-72587a4e0d9f"), xg::Guid("8f4196e6-2299-4fb5-b2e4-d5ced06e093f"), true);
}

void DebugRenderer::CreateBuffers(DebugRendererData& data)
{
	BufferData bdata{};
	bdata.Type = BufferType::VertexColor;
	
	bdata.VertexColorBuffer.Vertices.resize(1024);

	m_DebugBuffer = data.BufferManager->GetBuffer(bdata, xg::newGuid());
}

void DebugRenderer::Initialize(InitializableBasic* data)
{
	DebugRendererData& mdata = *(DebugRendererData*)data;

	CreateShaders(mdata);
	CreateBuffers(mdata);

	mR_AssetManager = mdata.AssetManager;
	mR_BufferManager = mdata.BufferManager;
	mR_Renderer = mdata.Renderer;
}

void DebugRenderer::Shutdown()
{
}

void DebugRenderer::Render()
{
	mR_Renderer->BindShader(m_DebugScreenShader);
	mR_Renderer->DrawBuffer(m_DebugBuffer);
}
