#include "pch.h"
#include "DeviceManager.h"

#include <vector>
#include <map>

#include "Data/DeviceManagerData.h"
#include "Utilities/VulkanUtils.h"
#include "Utilities/DeviceUtils.h"
#include <set>

#include "Debug/VulkanDebug.h"

void DeviceManager::PickPhysicalDevice(DeviceManagerData& data)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(data.Instance, &deviceCount, nullptr);

	if (deviceCount == 0) {
		
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(data.Instance, &deviceCount, devices.data());

	std::multimap<int, VkPhysicalDevice> candidates;

	for (const auto& device : devices) {
		if (DeviceUtils::IsDeviceSuitable(device, data.Surface, data.Type)) {
			int score = DeviceUtils::RateDeviceSuitability(device);
			candidates.insert(std::make_pair(score, device));
		}
	}

	if (candidates.rbegin()->first > 0) {
		m_PhysicalDevice = candidates.rbegin()->second;
	}
	else {
		VkLocalCheckF(VK_ERROR_UNKNOWN);
	}
}

void DeviceManager::CreateLogicalDevice(DeviceManagerData& data)
{
	QueueFamilyIndices indices = DeviceUtils::FindQueueFamilies(m_PhysicalDevice, data.Surface);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = queueCreateInfos.size();

	createInfo.pEnabledFeatures = &deviceFeatures;

	auto extensions = DeviceUtils::GetRequiredExtensions();
	createInfo.ppEnabledExtensionNames = extensions.data();
	createInfo.enabledExtensionCount = extensions.size();

	auto vlayers = VulkanUtils::GetValidationLayers();
	if (VulkanUtils::ValidationLayersEnabled()) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(vlayers.size());
		createInfo.ppEnabledLayerNames = vlayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	VkLocalCheckF(vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_LogicalDevice));
}

void DeviceManager::GetDeviceQueues(DeviceManagerData& data)
{
	QueueFamilyIndices indices = DeviceUtils::FindQueueFamilies(m_PhysicalDevice, data.Surface);

	vkGetDeviceQueue(m_LogicalDevice, indices.GraphicsFamily.value(), 0, &m_GraphicsQueue);
	vkGetDeviceQueue(m_LogicalDevice, indices.PresentFamily.value(), 0, &m_PresentQueue);
}

void DeviceManager::Initialize(InitializableBasic* data)
{
	DeviceManagerData& mdata = *(DeviceManagerData*)data;

	PickPhysicalDevice(mdata);
	CreateLogicalDevice(mdata);
	GetDeviceQueues(mdata);
}

void DeviceManager::Shutdown()
{
	vkDestroyDevice(m_LogicalDevice, NULL);
	m_PhysicalDevice = VK_NULL_HANDLE;
}
