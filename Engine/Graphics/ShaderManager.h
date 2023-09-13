#pragma once

#include "VarData.h"

#include <map>

#include <vulkan/vulkan.h>
#include <Guid.hpp>

#include "Basics/Initializable.h"

class Shader;
class ShaderCompiler;
class AssetManager;
struct ShaderData;

class ShaderManager : public IInitializable {
private:
	VkDevice mR_Device;

	ShaderCompiler* m_Compiler;

	AssetManager* mR_AssetManager;

	std::vector<std::pair<std::pair<xg::Guid, xg::Guid>, std::shared_ptr<Shader>>> m_Shaders;
	std::map<std::pair<xg::Guid, xg::Guid>, ShaderData*> m_ShaderData;
public:
	void EXPORT Initialize(InitializableBasic* data) override;
	void EXPORT Shutdown() override;

	std::shared_ptr<Shader> EXPORT GetShader(xg::Guid vasset, xg::Guid fasset);
};
