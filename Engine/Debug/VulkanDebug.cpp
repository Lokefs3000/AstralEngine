#include "pch.h"
#include "VulkanDebug.h"

#include <vulkan/vk_enum_string_helper.h>

#include "LogPrinter.h"

void VulkanDebug::VkErrorCheck(VkResult result, bool fatal)
{
	if (result != VK_SUCCESS) {
		const char* err = string_VkResult(result);

		if (fatal) {
			ShowMessageBox(LogPrinterSeverity::Fatal, LogPrinterButtons::Ok, err);
			exit(0);
		}
		else {
			Error(err);
		}
	}
}
