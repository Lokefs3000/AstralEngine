#include "pch.h"
#include "SwapChainManager.h"

#include <mutex>

#include "Data/SwapChainManagerData.h"
#include "Data/DeviceManagerData.h"

#include "Utilities/SwapChainUtils.h"
#include "Utilities/DeviceUtils.h"

#include "Graphics/Window.h"

#include "Debug/VulkanDebug.h"

void SwapChainManager::CreateSwapChain(SwapChainManagerData& data)
{
    SwapChainSupportDetails swapChainSupport = SwapChainUtils::QuerySwapChainSupport(data.PhysicalDevice, data.Surface);

    VkSurfaceFormatKHR surfaceFormat = SwapChainUtils::ChooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = SwapChainUtils::ChooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = SwapChainUtils::ChooseSwapExtent(data.TargetWindow, swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = data.Surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = DeviceUtils::FindQueueFamilies(data.PhysicalDevice, data.Surface);
    uint32_t queueFamilyIndices[] = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

    if (indices.GraphicsFamily != indices.PresentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    VkLocalCheckF(vkCreateSwapchainKHR(data.Device, &createInfo, nullptr, &m_SwapChain));

    m_SwapChainImageFormat = surfaceFormat.format;
    m_SwapChainExtent = extent;
}

void SwapChainManager::CreateSwapChainImages(SwapChainManagerData& data)
{
    uint32_t imageCount = 0;

    VkLocalCheckF(vkGetSwapchainImagesKHR(data.Device, m_SwapChain, &imageCount, nullptr));
    m_SwapChainImages.resize(imageCount);
    VkLocalCheckF(vkGetSwapchainImagesKHR(data.Device, m_SwapChain, &imageCount, m_SwapChainImages.data()));
}

void SwapChainManager::CreateSwapChainImageViews(SwapChainManagerData& data)
{
    m_SwapChainImageViews.resize(m_SwapChainImages.size());

    for (size_t i = 0; i < m_SwapChainImages.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = m_SwapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = m_SwapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        VkLocalCheckF(vkCreateImageView(data.Device, &createInfo, nullptr, &m_SwapChainImageViews[i]));
    }
}

void SwapChainManager::CreateSwapChainFramebuffers(SwapChainManagerData& data)
{
    m_SwapChainFramebuffers.resize(m_SwapChainImageViews.size());

    for (size_t i = 0; i < m_SwapChainImageViews.size(); i++) {
        VkImageView attachments[] = {
            m_SwapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = data.RenderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = m_SwapChainExtent.width;
        framebufferInfo.height = m_SwapChainExtent.height;
        framebufferInfo.layers = 1;

        VkLocalCheckF(vkCreateFramebuffer(data.Device, &framebufferInfo, nullptr, &m_SwapChainFramebuffers[i]));
    }
}

void SwapChainManager::RecreateSwapChain(bool isResized)
{
    VkLocalCheckF(vkDeviceWaitIdle(mR_Device));

    int w, h;
    SDL_GetWindowSizeInPixels(mR_TargetWindow->GetSDLWindow(), &w, &h);

    if (mR_TargetWindow->IsMinimized() || w == 0 || h == 0) {
        std::unique_lock<std::mutex> lock(m_MinimizeMutex);
        m_MinimizeCondition.wait(lock, [this] { return !mR_TargetWindow->IsMinimized(); });
    }

    SwapChainManagerData mdata;
    mdata.Device = mR_Device;
    mdata.PhysicalDevice = mR_PhysicalDevice;
    mdata.RenderPass = mR_RenderPass;
    mdata.Surface = mR_Surface;
    mdata.TargetWindow = mR_TargetWindow;

    DestroySwapChain();

    CreateSwapChain(mdata);
    CreateSwapChainImages(mdata);
    CreateSwapChainImageViews(mdata);
    CreateSwapChainFramebuffers(mdata);
}

void SwapChainManager::OnMinimizeEnd()
{
    m_MinimizeCondition.notify_all();
}

void SwapChainManager::DestroySwapChain()
{
    for (auto framebuffer : m_SwapChainFramebuffers) {
        vkDestroyFramebuffer(mR_Device, framebuffer, nullptr);
    }

    for (auto imageView : m_SwapChainImageViews) {
        vkDestroyImageView(mR_Device, imageView, nullptr);
    }

    vkDestroySwapchainKHR(mR_Device, m_SwapChain, NULL);
}

void SwapChainManager::Initialize(InitializableBasic* data)
{
    SwapChainManagerData& mdata = *(SwapChainManagerData*)data;

    CreateSwapChain(mdata);
    CreateSwapChainImages(mdata);
    CreateSwapChainImageViews(mdata);
    CreateSwapChainFramebuffers(mdata);

    mR_PhysicalDevice = mdata.PhysicalDevice;
    mR_Device = mdata.Device;
    mR_RenderPass = mdata.RenderPass;
    mR_Surface = mdata.Surface;
    mR_TargetWindow = mdata.TargetWindow;
}

void SwapChainManager::Shutdown()
{
    DestroySwapChain();
}
