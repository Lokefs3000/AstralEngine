#pragma once

#include "VarData.h"

#include <vulkan/vulkan.h>

#include <vector>

#include "Basics/Initializable.h"

struct ShaderData;

class Shader : public IInitializable {
private:
	VkShaderModule m_VertexShader;
	VkShaderModule m_FragmentShader;

	VkPipelineLayout m_Layout;
	VkPipeline m_Pipeline;

	VkDevice mR_Device;

	void CreateShaders(ShaderData& data);
	void CreatePipelineLayout(ShaderData& data);
	void CreatePipeline(ShaderData& data);

	VkShaderModule CreateShaderModule(ShaderData& data, const std::vector<uint32_t>& code);
public:
	void EXPORT Initialize(InitializableBasic* data) override;
	void EXPORT Shutdown() override;

	VkShaderModule GetVertexShader() { return m_VertexShader; };
	VkShaderModule GetFragmentShader() { return m_FragmentShader; };

	VkPipelineLayout GetPipelineLayout() { return m_Layout; }
	VkPipeline GetPipeline() { return m_Pipeline; }
};