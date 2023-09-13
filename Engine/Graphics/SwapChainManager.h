#pragma once

#include "VarData.h"

#include <mutex>
#include <vector>

#include <vulkan/vulkan.h>

#include "Basics/Initializable.h"

struct SwapChainManagerData;
class Window;

class SwapChainManager : public IInitializable {
private:
	VkSwapchainKHR m_SwapChain;
	std::vector<VkImage> m_SwapChainImages;
	VkFormat m_SwapChainImageFormat;
	VkExtent2D m_SwapChainExtent;
	std::vector<VkImageView> m_SwapChainImageViews;
	std::vector<VkFramebuffer> m_SwapChainFramebuffers;

	std::mutex m_MinimizeMutex;
	std::condition_variable m_MinimizeCondition;

	VkPhysicalDevice mR_PhysicalDevice;
	VkDevice mR_Device;
	VkRenderPass mR_RenderPass;
	VkSurfaceKHR mR_Surface;
	Window* mR_TargetWindow;

	void CreateSwapChain(SwapChainManagerData& data);
	void CreateSwapChainImages(SwapChainManagerData& data);
	void CreateSwapChainImageViews(SwapChainManagerData& data);
	void CreateSwapChainFramebuffers(SwapChainManagerData& data);

	void DestroySwapChain();
public:
	void EXPORT Initialize(InitializableBasic* data) override;
	void EXPORT Shutdown() override;

	void EXPORT RecreateSwapChain(bool isResized);
	void EXPORT OnMinimizeEnd();

	VkSwapchainKHR GetSwapChain() { return m_SwapChain; };
	std::vector<VkImage> GetSwapChainImages() { return m_SwapChainImages; };
	VkFormat GetSwapChainImageFormat() { return m_SwapChainImageFormat; };
	VkExtent2D GetSwapChainExtent() { return m_SwapChainExtent; };
	std::vector<VkImageView> GetSwapChainImageViews() { return m_SwapChainImageViews; };
	std::vector<VkFramebuffer> GetSwapChainFramebuffers() { return m_SwapChainFramebuffers; }
};