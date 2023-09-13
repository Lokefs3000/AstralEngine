#pragma once

#include <vector>

#include "Basics/Initializable.h"

#include <vulkan/vulkan.h>

class Window;

struct SwapChainManagerData : public InitializableBasic {
public:
    Window* TargetWindow;
    VkSurfaceKHR Surface;
    VkDevice Device;
    VkPhysicalDevice PhysicalDevice;
    VkRenderPass RenderPass;
};

struct SwapChainSupportDetails {
public:
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};