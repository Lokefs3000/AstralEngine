#pragma once

#include "VarData.h"

#include <vulkan/vulkan.h>

#include "Basics/Initializable.h"

struct DeviceManagerData;

class DeviceManager : public IInitializable {
private:
	VkPhysicalDevice m_PhysicalDevice;
	VkDevice m_LogicalDevice;

	VkQueue m_GraphicsQueue;
	VkQueue m_PresentQueue;

	void PickPhysicalDevice(DeviceManagerData& data);
	void CreateLogicalDevice(DeviceManagerData& data);
	void GetDeviceQueues(DeviceManagerData& data);
public:
	void EXPORT Initialize(InitializableBasic* data) override;
	void EXPORT Shutdown() override;

	VkPhysicalDevice GetPhysicalDevice() { return m_PhysicalDevice; }
	VkDevice GetLogicalDevice() { return m_LogicalDevice; }
	VkQueue GetGraphicsQueue() { return m_GraphicsQueue; }
	VkQueue GetPresentQueue() { return m_PresentQueue; }
};