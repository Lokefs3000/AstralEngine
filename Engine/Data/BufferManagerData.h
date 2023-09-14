#pragma once

#include "VarData.h"

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "Basics/Initializable.h"

struct BufferManagerData : public InitializableBasic {
public:
	VkDevice Device;
	VkPhysicalDevice PhysicalDevice;
	VkCommandPool CommandPool;
	VkQueue GraphicsQueue;
};

struct BufferCreateData {
public:
	std::vector<glm::vec3> PositionsV3;
	std::vector<glm::vec4> PositionsV4;

	std::vector<uint64_t> Indices;

	std::vector<glm::vec4> Colors;
};