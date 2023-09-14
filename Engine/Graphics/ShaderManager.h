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
	VkInstance mR_Instance;
	VkRenderPass mR_RenderPass;
	VkFormat mR_Format;

	ShaderCompiler* m_Compiler;

	AssetManager* mR_AssetManager;

	std::vector<std::pair<std::pair<xg::Guid, xg::Guid>, std::shared_ptr<Shader>>> m_Shaders;
public:
	void EXPORT Initialize(InitializableBasic* data) override;
	void EXPORT Shutdown() override;

	std::shared_ptr<Shader> EXPORT GetShader(xg::Guid vasset, xg::Guid fasset, bool is2D);
};
