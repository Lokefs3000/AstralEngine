#pragma once

#include "VarData.h"

#include <vulkan/vulkan.h>

#include "Basics/Initializable.h"

class SwapChainManager;

struct RendererCoreData : public InitializableBasic {
public:
	SwapChainManager* SwapChainManager;
	VkSurfaceKHR Surface;
	VkPhysicalDevice PhysicalDevice;
	VkRenderPass RenderPass;
	VkDevice Device;
	VkQueue GraphicsQueue;
	VkQueue PresentQueue;

	uint32_t MaxFramesInFlight;
};