#pragma once

#include "VarData.h"

#include <memory>

#include <vulkan/vulkan.h>
#include <Guid.hpp>

#include "Basics/Initializable.h"

struct BufferData;
class Buffer;

class BufferManager : public IInitializable {
private:
	std::vector<std::pair<xg::Guid, std::shared_ptr<Buffer>>> m_Buffers;

	VkDevice mR_Device;
	VkPhysicalDevice mR_PhysicalDevice;
	VkCommandPool mR_CommandPool;
	VkQueue mR_GraphicsQueue;
public:
	void EXPORT Initialize(InitializableBasic* data) override;
	void EXPORT Shutdown() override;

	std::shared_ptr<Buffer> GetBuffer(BufferData& data, xg::Guid id);
};