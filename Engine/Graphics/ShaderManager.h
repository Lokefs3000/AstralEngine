#pragma once

#include "VarData.h"

#include <vulkan/vulkan.h>
#include <Guid.hpp>

#include "Basics/Initializable.h"

class Shader;
class ShaderCompiler;

class ShaderManager : public IInitializable {
private:
	VkDevice mR_Device;

	ShaderCompiler* m_Compiler;

	std::vector<std::pair<xg::Guid, std::shared_ptr<Shader>>> m_Shaders;
public:
	void EXPORT Initialize(InitializableBasic* data) override;
	void EXPORT Shutdown() override;

	std::shared_ptr<Shader> EXPORT GetShader(xg::Guid asset);
};
