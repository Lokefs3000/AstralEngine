#pragma once

#include "VarData.h"

#include <vulkan/vulkan.h>

class BufferUtils {
public:
	static uint32_t EXPORT FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
	static void EXPORT CreateBuffer(VkDevice device, VkPhysicalDevice pdevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	static void EXPORT CopyBuffer(VkDevice device, VkCommandPool pool, VkQueue gqueue, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};