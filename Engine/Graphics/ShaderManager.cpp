#include "pch.h"
#include "ShaderManager.h"

#include "Shader/ShaderCompiler.h"
#include "Shader/ShaderWorkerData.h"
#include "Data/ShaderManagerData.h"
#include "Shader.h"

#include "Assets/AssetManager.h"

void ShaderManager::Initialize(InitializableBasic* data)
{
	ShaderManagerData& mdata = *(ShaderManagerData*)data;

	m_Compiler = new ShaderCompiler(mdata.WorkerCount);

	mR_AssetManager = mdata.AssetManager;
	mR_Device = mdata.Device;
	mR_Instance = mdata.Instance;
	mR_RenderPass = mdata.RenderPass;
	mR_Format = mdata.Format;
}

void ShaderManager::Shutdown()
{
}

std::shared_ptr<Shader> ShaderManager::GetShader(xg::Guid vasset, xg::Guid fasset, bool is2D)
{
	for (size_t i = 0; i < m_Shaders.size(); i++)
		if (m_Shaders[i].first.first == vasset && m_Shaders[i].first.second == vasset)
			return m_Shaders[i].second;

	ShaderData sdata;
	sdata.Instance = mR_Instance;
	sdata.Device = mR_Device;
	sdata.RenderPass = mR_RenderPass;
	sdata.Format = mR_Format;
	sdata.BlendEnabled = false;
	sdata.Width = 1;
	sdata.Height = 1;
	sdata.Compiler = m_Compiler;
	sdata.VAsset = mR_AssetManager->GetSource(vasset);
	sdata.FAsset = mR_AssetManager->GetSource(fasset);
	sdata.Use2DBuffers = is2D;

	std::shared_ptr<Shader> shader = std::make_shared<Shader>();
	shader->Initialize(&sdata);

	m_Shaders.push_back(std::make_pair(std::make_pair(vasset, fasset), shader));
	return shader;
}
