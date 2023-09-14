#pragma once

#include "VarData.h"

#include <vector>

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "Basics/Initializable.h"

enum class BufferType {
	Unkown,
	PositionIndex,
	VertexColor
};

struct BufferData : InitializableBasic {
public:
	struct _PositionIndexBuffer
	{
	public:
		std::vector<glm::vec3> Positions;
		std::vector<uint64_t> Indices;
	};
	_PositionIndexBuffer PositionIndexBuffer;

	struct _VertexColorBuffer
	{
	public:
		struct Vertex {
		public:
			glm::vec4 Vertice;
			glm::vec4 Color;
		};

		std::vector<Vertex> Vertices;
	};
	_VertexColorBuffer VertexColorBuffer;

	BufferType Type;

	VkDevice Device;
	VkPhysicalDevice PhysicalDevice;
	VkCommandPool CommandPool;
	VkQueue GraphicsQueue;
};