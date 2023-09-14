#pragma once

#include <vector>
#include <string>

#include <vulkan/vulkan.h>

#include "Basics/Initializable.h"

class AssetManager;
class ShaderCompiler;

struct ShaderManagerData : public InitializableBasic
{
public:
	VkInstance Instance;
	VkDevice Device;
	VkRenderPass RenderPass;
	VkFormat Format;

	AssetManager* AssetManager;

	uint8_t WorkerCount;
};

struct ShaderData : public InitializableBasic
{
public:
	VkInstance Instance;
	VkDevice Device;
	VkRenderPass RenderPass;

	ShaderCompiler* Compiler;

	std::u16string VAsset;
	std::u16string FAsset;

	VkFormat Format;

	bool BlendEnabled;

	uint32_t Width;
	uint32_t Height;

	std::vector<uint32_t> VertBytecode;
	std::vector<uint32_t> FragBytecode;

	bool Use2DBuffers;
};