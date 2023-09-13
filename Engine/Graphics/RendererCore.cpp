#include "pch.h"
#include "RendererCore.h"

#include <iostream>

#include "Data/RendererCoreData.h"
#include "Data/DeviceManagerData.h"

#include "Utilities/DeviceUtils.h"

#include "SwapChainManager.h"

#include "Debug/VulkanDebug.h"

void RendererCore::CreateCommandPool(RendererCoreData& data)
{
	QueueFamilyIndices queueFamilyIndices = DeviceUtils::FindQueueFamilies(data.PhysicalDevice, data.Surface);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.GraphicsFamily.value();

	VkLocalCheckF(vkCreateCommandPool(data.Device, &poolInfo, nullptr, &m_CommandPool));
}

void RendererCore::CreateCommandBuffers(RendererCoreData& data)
{
	m_CommandBuffer.resize(data.MaxFramesInFlight);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = m_CommandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = data.MaxFramesInFlight;

	VkLocalCheckF(vkAllocateCommandBuffers(data.Device, &allocInfo, m_CommandBuffer.data()));
}

void RendererCore::CreateSyncObjects(RendererCoreData& data)
{
	m_ImageAvailableSemaphore.resize(data.MaxFramesInFlight);
	m_RenderFinishedSemaphore.resize(data.MaxFramesInFlight);
	m_InFlightFence.resize(data.MaxFramesInFlight);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < data.MaxFramesInFlight; i++) {
		VkLocalCheckF(vkCreateSemaphore(data.Device, &semaphoreInfo, nullptr, &m_ImageAvailableSemaphore[i]));
		VkLocalCheckF(vkCreateSemaphore(data.Device, &semaphoreInfo, nullptr, &m_RenderFinishedSemaphore[i]));
		VkLocalCheckF(vkCreateFence(data.Device, &fenceInfo, nullptr, &m_InFlightFence[i]));
	}
}

void RendererCore::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0; // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional

	VkLocalCheckF(vkBeginCommandBuffer(commandBuffer, &beginInfo))

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = mR_RenderPass;
	renderPassInfo.framebuffer = mR_SwapChainManager->GetSwapChainFramebuffers()[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = mR_SwapChainManager->GetSwapChainExtent();

	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void RendererCore::StopCommandBuffer(VkCommandBuffer commandBuffer)
{
	vkCmdEndRenderPass(commandBuffer);

	VkLocalCheckF(vkEndCommandBuffer(commandBuffer));
}

void RendererCore::Initialize(InitializableBasic* data)
{
	RendererCoreData& mdata = *(RendererCoreData*)data;

	CreateCommandPool(mdata);
	CreateCommandBuffers(mdata);
	CreateSyncObjects(mdata);

	mR_Device = mdata.Device;
	mR_RenderPass = mdata.RenderPass;
	mR_SwapChainManager = mdata.SwapChainManager;
	mR_GraphicsQueue = mdata.GraphicsQueue;
	mR_PresentQueue = mdata.PresentQueue;
	mR_MaxFramesInFlight = mdata.MaxFramesInFlight;
}

void RendererCore::Shutdown()
{
	for (size_t i = 0; i < mR_MaxFramesInFlight; i++) {
		vkDestroySemaphore(mR_Device, m_RenderFinishedSemaphore[i], nullptr);
		vkDestroySemaphore(mR_Device, m_ImageAvailableSemaphore[i], nullptr);
		vkDestroyFence(mR_Device, m_InFlightFence[i], nullptr);
	}


	vkDestroyCommandPool(mR_Device, m_CommandPool, nullptr);
}

void RendererCore::DrawFrame()
{
	VkLocalCheckF(vkWaitForFences(mR_Device, 1, &m_InFlightFence[m_CurrentFrame], VK_TRUE, UINT64_MAX));

	VkResult result = vkAcquireNextImageKHR(mR_Device, mR_SwapChainManager->GetSwapChain(), UINT64_MAX, m_ImageAvailableSemaphore[m_CurrentFrame], VK_NULL_HANDLE, &m_ImageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		mR_SwapChainManager->RecreateSwapChain(false);
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		VkLocalCheckF(result);
	}

	VkLocalCheckF(vkResetFences(mR_Device, 1, &m_InFlightFence[m_CurrentFrame]));

	VkLocalCheckF(vkResetCommandBuffer(m_CommandBuffer[m_CurrentFrame], 0));

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	RecordCommandBuffer(m_CommandBuffer[m_CurrentFrame], m_ImageIndex);
}

void RendererCore::EndFrame()
{
	StopCommandBuffer(m_CommandBuffer[m_CurrentFrame]);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphore[m_CurrentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_CommandBuffer[m_CurrentFrame];

	VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphore[m_CurrentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	VkLocalCheckF(vkQueueSubmit(mR_GraphicsQueue, 1, &submitInfo, m_InFlightFence[m_CurrentFrame]));

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { mR_SwapChainManager->GetSwapChain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &m_ImageIndex;
	presentInfo.pResults = nullptr; // Optional

	VkResult result = vkQueuePresentKHR(mR_PresentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
		mR_SwapChainManager->RecreateSwapChain(false);
	}
	else if (result != VK_SUCCESS) {
		VkLocalCheckF(result);
	}

	m_CurrentFrame = (m_CurrentFrame + 1) % mR_MaxFramesInFlight;
}
