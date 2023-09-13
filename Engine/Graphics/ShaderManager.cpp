#include "pch.h"
#include "ShaderManager.h"

#include "Shader/ShaderCompiler.h"
#include "Shader/ShaderWorkerData.h"
#include "Data/ShaderManagerData.h"
#include "Shader.h"

void ShaderManager::Initialize(InitializableBasic* data)
{
	ShaderManagerData& mdata = *(ShaderManagerData*)data;

	m_Compiler = new ShaderCompiler(mdata.WorkerCount);
}

void ShaderManager::Shutdown()
{
}

std::shared_ptr<Shader> ShaderManager::GetShader(xg::Guid asset)
{
	for (size_t i = 0; i < m_Shaders.size(); i++)
		if (m_Shaders[i].first == asset)
			return m_Shaders[i].second;

	std::shared_ptr<Shader> shader = std::make_shared<Shader>();

	std::u16string vSource = u"";
	std::u16string fSource = u"";

	ShaderData data;

	data.Device = mR_Device;
	data.Instance = VK_NULL_HANDLE;
	data.BlendEnabled = false;

	m_Compiler->AddShader(vSource, VERTEX_SHADER, [this, &data, &shader](ShaderWorkerResult result) {
		data.VertBytecode = result.Bytecode;

		if (data.VertBytecode.size() > 0 && data.FragBytecode.size() > 0) {
			shader->Initialize(&data);
		}
	});

	m_Compiler->AddShader(fSource, FRAGMENT_SHADER, [this, &data, &shader](ShaderWorkerResult result) {
		data.FragBytecode = result.Bytecode;

		if (data.VertBytecode.size() > 0 && data.FragBytecode.size() > 0) {
			shader->Initialize(&data);
		}
	});

	m_Shaders.push_back(std::make_pair(asset, shader));
	return shader;
}
