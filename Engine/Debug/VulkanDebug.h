#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

#include "LogPrinter.h"

class VulkanDebug {
public:
	static void VkErrorCheck(VkResult result, bool fatal = false);
};

#define VkLocalCheckF(result) \
	if (result != VK_SUCCESS) { \
		const char* err = string_VkResult(result); \
		ShowMessageBox(LogPrinterSeverity::Fatal, LogPrinterButtons::Ok, err); \
		exit(result); \
	}

#define VkLocalCheck(result) \
	if (result != VK_SUCCESS) { \
		const char* err = string_VkResult(result); \
		Error(err); \
		return; \
	}