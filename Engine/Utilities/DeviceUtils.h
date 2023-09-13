#pragma once

#include "VarData.h"

#include <vector>

#include <vulkan/vulkan.h>

struct QueueFamilyIndices;

class DeviceUtils {
public:
	static std::vector<const char*> GetRequiredExtensions();
	static bool EXPORT IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, uint32_t type);
	static bool EXPORT CheckDeviceExtensionSupport(VkPhysicalDevice device);
	static int EXPORT RateDeviceSuitability(VkPhysicalDevice device);
	static QueueFamilyIndices EXPORT FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
};