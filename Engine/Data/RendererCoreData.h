#pragma once

#include "VarData.h"

#include <vulkan/vulkan.h>

#include "Basics/Initializable.h"

class SwapChainManager;
class AssetManager;

struct RendererCoreData : public InitializableBasic {
public:
	SwapChainManager* SwapChainManager;
	VkSurfaceKHR Surface;
	VkPhysicalDevice PhysicalDevice;
	VkRenderPass RenderPass;
	VkDevice Device;
	VkQueue GraphicsQueue;
	VkQueue PresentQueue;

	AssetManager* AssetManager;

	uint32_t MaxFramesInFlight;
};