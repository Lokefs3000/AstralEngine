#pragma once

#include "VarData.h"

#include <vector>
#include <vulkan/vulkan.h>

class VulkanUtils {
public:
	static std::vector<const char*> EXPORT RetrieveRequiredExtensions();

    static std::vector<const char*> EXPORT GetValidationLayers();
    static bool EXPORT ValidationLayersEnabled();

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
    static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
    static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
    static void FillDebugMessengerInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
};