#include "pch.h"
#include "Buffer.h"

#include "Data/BufferData.h"

#include "Debug/VulkanDebug.h"

#include "Utilities/BufferUtils.h"

void Buffer::CreatePositionIndexBuffer(BufferData& data)
{
	VkDeviceSize bufferSize = sizeof(data.PositionIndexBuffer.Positions[0]) * data.PositionIndexBuffer.Positions.size();
	
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	BufferUtils::CreateBuffer(data.Device, data.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* pdata;
	vkMapMemory(data.Device, stagingBufferMemory, 0, bufferSize, 0, &pdata);
	memcpy(pdata, data.PositionIndexBuffer.Positions.data(), (size_t)bufferSize);
	vkUnmapMemory(data.Device, stagingBufferMemory);

	BufferUtils::CreateBuffer(data.Device, data.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VertexBuffer, m_VertexMemory);
	BufferUtils::CopyBuffer(data.Device, data.CommandPool, data.GraphicsQueue, stagingBuffer, m_VertexBuffer, bufferSize);

	vkDestroyBuffer(data.Device, stagingBuffer, nullptr);
	vkFreeMemory(data.Device, stagingBufferMemory, nullptr);

	////////////////////////////////////////////////////////////////////////////

	bufferSize = sizeof(data.PositionIndexBuffer.Indices[0]) * data.PositionIndexBuffer.Indices.size();
	BufferUtils::CreateBuffer(data.Device, data.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	vkMapMemory(data.Device, stagingBufferMemory, 0, bufferSize, 0, &pdata);
	memcpy(pdata, data.PositionIndexBuffer.Indices.data(), (size_t)bufferSize);
	vkUnmapMemory(data.Device, stagingBufferMemory);

	BufferUtils::CreateBuffer(data.Device, data.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_IndexBuffer, m_IndexMemory);
	BufferUtils::CopyBuffer(data.Device, data.CommandPool, data.GraphicsQueue, stagingBuffer, m_IndexBuffer, bufferSize);

	vkDestroyBuffer(data.Device, stagingBuffer, nullptr);
	vkFreeMemory(data.Device, stagingBufferMemory, nullptr);

	m_Vertices = data.PositionIndexBuffer.Positions.size();
	m_Indices = data.PositionIndexBuffer.Indices.size();
}

void Buffer::CreateVertexColorBuffer(BufferData& data)
{
	VkDeviceSize bufferSize = sizeof(data.VertexColorBuffer.Vertices[0]) * data.VertexColorBuffer.Vertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	BufferUtils::CreateBuffer(data.Device, data.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* pdata;
	vkMapMemory(data.Device, stagingBufferMemory, 0, bufferSize, 0, &pdata);
	memcpy(pdata, data.VertexColorBuffer.Vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(data.Device, stagingBufferMemory);

	BufferUtils::CreateBuffer(data.Device, data.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VertexBuffer, m_VertexMemory);
	BufferUtils::CopyBuffer(data.Device, data.CommandPool, data.GraphicsQueue, stagingBuffer, m_VertexBuffer, bufferSize);

	vkDestroyBuffer(data.Device, stagingBuffer, nullptr);
	vkFreeMemory(data.Device, stagingBufferMemory, nullptr);

	m_Vertices = data.VertexColorBuffer.Vertices.size();
	m_Indices = 0;
}

void Buffer::Initialize(InitializableBasic* data)
{
	BufferData& mdata = *(BufferData*)data;

	switch (mdata.Type)
	{
	case BufferType::PositionIndex:
		CreatePositionIndexBuffer(mdata);
		break;
	case BufferType::VertexColor:
		CreateVertexColorBuffer(mdata);
		break;
	default:
		break;
	}

	mR_Device = mdata.Device;
}

void Buffer::Shutdown()
{
	if (m_IndexBuffer != NULL) {
		vkDestroyBuffer(mR_Device, m_IndexBuffer, NULL);
		vkFreeMemory(mR_Device, m_IndexMemory, nullptr);
	}

	if (m_VertexBuffer != NULL) {
		vkDestroyBuffer(mR_Device, m_VertexBuffer, NULL);
		vkFreeMemory(mR_Device, m_VertexMemory, nullptr);
	}
}
