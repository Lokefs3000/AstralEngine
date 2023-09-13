#pragma once

#include "VarData.h"

#include <vector>

#include <vulkan/vulkan.h>

#include "Basics/Initializable.h"

struct RendererCoreData;
class SwapChainManager;

class RendererCore : public IInitializable {
private:
	VkCommandPool m_CommandPool;
	std::vector<VkCommandBuffer> m_CommandBuffer;

	std::vector<VkSemaphore> m_ImageAvailableSemaphore;
	std::vector<VkSemaphore> m_RenderFinishedSemaphore;
	std::vector<VkFence> m_InFlightFence;

	uint32_t m_CurrentFrame = 0;

	VkDevice mR_Device;
	VkRenderPass mR_RenderPass;
	VkQueue mR_GraphicsQueue;
	VkQueue mR_PresentQueue;
	uint32_t mR_MaxFramesInFlight;

	SwapChainManager* mR_SwapChainManager;

	void CreateCommandPool(RendererCoreData& data);
	void CreateCommandBuffers(RendererCoreData& data);
	void CreateSyncObjects(RendererCoreData& data);

	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	void StopCommandBuffer(VkCommandBuffer commandBuffer);
public:
	void EXPORT Initialize(InitializableBasic* data) override;
	void EXPORT Shutdown() override;

	void EXPORT DrawFrame();
};