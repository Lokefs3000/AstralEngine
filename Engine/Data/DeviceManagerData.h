#pragma once

#include <string>
#include <optional>

#include <vulkan/vulkan.h>

#include "Basics/Initializable.h"

struct DeviceManagerData : public InitializableBasic {
public:
	VkInstance Instance;
	VkSurfaceKHR Surface;
	uint32_t Type;
};

struct QueueFamilyIndices {
	std::optional<uint32_t> GraphicsFamily;
	std::optional<uint32_t> PresentFamily;

	bool IsComplete() {
		return GraphicsFamily.has_value() && PresentFamily.has_value();
	}
};