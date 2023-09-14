#pragma once

#include "VarData.h"

#include <vector>
#include <memory>

#include <vulkan/vulkan.h>

#include "Basics/Initializable.h"

struct RendererCoreData;
class SwapChainManager;
class Shader;
class Buffer;

class RendererCore : public IInitializable {
private:
	VkCommandPool m_CommandPool;
	std::vector<VkCommandBuffer> m_CommandBuffer;

	std::vector<VkSemaphore> m_ImageAvailableSemaphore;
	std::vector<VkSemaphore> m_RenderFinishedSemaphore;
	std::vector<VkFence> m_InFlightFence;

	uint32_t m_CurrentFrame = 0;
	uint32_t m_ImageIndex = 0;

	VkDevice mR_Device;
	VkRenderPass mR_RenderPass;
	VkQueue mR_GraphicsQueue;
	VkQueue mR_PresentQueue;
	uint32_t mR_MaxFramesInFlight;

	bool m_PipelineBoundThisRun;

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
	void EXPORT EndFrame();

	void EXPORT BindShader(std::shared_ptr<Shader> shader);
	void EXPORT DrawBuffer(std::shared_ptr<Buffer> buffer);
	void EXPORT DrawBufferSpecified(std::shared_ptr<Buffer> buffer, uint64_t start, uint64_t count);

	VkCommandPool GetCommandPool() { return m_CommandPool; }
};