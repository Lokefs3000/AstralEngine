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
}

void ShaderManager::Shutdown()
{
}

std::shared_ptr<Shader> ShaderManager::GetShader(xg::Guid vasset, xg::Guid fasset)
{
	for (size_t i = 0; i < m_Shaders.size(); i++)
		if (m_Shaders[i].first.first == vasset && m_Shaders[i].first.second == vasset)
			return m_Shaders[i].second;


}
