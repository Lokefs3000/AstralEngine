#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "Basics/Initializable.h"

class AssetManager;

struct ShaderManagerData : public InitializableBasic
{
public:
	VkInstance Instance;
	VkDevice Device;
	VkRenderPass RenderPass;

	AssetManager* AssetManager;

	uint8_t WorkerCount;
};

struct ShaderData : public InitializableBasic
{
public:
	VkInstance Instance;
	VkDevice Device;
	VkRenderPass RenderPass;

	VkFormat Format;

	bool BlendEnabled;

	uint32_t Width;
	uint32_t Height;

	std::vector<uint32_t> VertBytecode;
	std::vector<uint32_t> FragBytecode;
};