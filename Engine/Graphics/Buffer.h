#pragma once

#include "VarData.h"

#include <vulkan/vulkan.h>

#include "Basics/Initializable.h"

struct BufferData;

class Buffer : public IInitializable {
private:
	VkBuffer m_VertexBuffer;
	VkDeviceMemory m_VertexMemory;

	VkBuffer m_IndexBuffer;
	VkDeviceMemory m_IndexMemory;

	uint64_t m_Vertices;
	uint64_t m_Indices;

	VkDevice mR_Device;

	void CreatePositionIndexBuffer(BufferData& data);
	void CreateVertexColorBuffer(BufferData& data);
public:
	void EXPORT Initialize(InitializableBasic* data) override;
	void EXPORT Shutdown() override;

	VkBuffer GetVertexBuffer() { return m_VertexBuffer; }
	VkBuffer GetIndexBuffer() { return m_IndexBuffer; }

	uint64_t GetVerticeCount() { return m_Vertices; }
	uint64_t GetIndiceCount() { return m_Indices; }
};