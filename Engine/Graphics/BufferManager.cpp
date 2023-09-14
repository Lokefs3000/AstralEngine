#include "pch.h"
#include "BufferManager.h"

#include "Data/BufferManagerData.h"
#include "Data/BufferData.h"

#include "Buffer.h"

void BufferManager::Initialize(InitializableBasic* data)
{
	BufferManagerData& mdata = *(BufferManagerData*)data;

	mR_Device = mdata.Device;
	mR_PhysicalDevice = mdata.PhysicalDevice;
	mR_CommandPool = mdata.CommandPool;
	mR_GraphicsQueue = mdata.GraphicsQueue;
}

void BufferManager::Shutdown()
{
}

std::shared_ptr<Buffer> BufferManager::GetBuffer(BufferData& data, xg::Guid id)
{
	for (size_t i = 0; i < m_Buffers.size(); i++)
		if (m_Buffers[i].first == id)
			return m_Buffers[i].second;

	std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>();

	data.Device = mR_Device;
	data.PhysicalDevice = mR_PhysicalDevice;
	data.GraphicsQueue = mR_GraphicsQueue;
	data.CommandPool = mR_CommandPool;

	buffer->Initialize(&data);

	return buffer;
}
