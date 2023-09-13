#include "pch.h"
#include "GraphicsCore.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include "Data/GraphicsCoreData.h"
#include "Data/DeviceManagerData.h"
#include "Data/ShaderManagerData.h"
#include "Data/SwapChainManagerData.h"
#include "Data/RendererCoreData.h"
#include "Data/DebugRendererData.h"

#include "DeviceManager.h"
#include "SwapChainManager.h"
#include "ShaderManager.h"
#include "RendererCore.h"
#include "Window.h"
#include "DebugRenderer.h"

#include "Utilities/VulkanUtils.h"
#include "Utilities/SwapChainUtils.h"

#include "Debug/VulkanDebug.h"

void GraphicsCore::CreateInstance(GraphicsCoreData& data)
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = data.ProductName.c_str();
    appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    appInfo.pEngineName = "AstralEngine1";
    appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    appInfo.apiVersion = USE_VULKAN_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    std::vector<const char*> extensions = VulkanUtils::RetrieveRequiredExtensions();
    createInfo.enabledExtensionCount = extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();

    std::vector<const char*> vlayers = VulkanUtils::GetValidationLayers();
    if (VulkanUtils::ValidationLayersEnabled()) {
        createInfo.enabledLayerCount = vlayers.size();
        createInfo.ppEnabledLayerNames = vlayers.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        VulkanUtils::FillDebugMessengerInfo(debugCreateInfo);
        createInfo.pNext = &debugCreateInfo;
    }

    VkLocalCheckF(vkCreateInstance(&createInfo, nullptr, &m_Instance));
}

void GraphicsCore::CreateDebugMessenger()
{
    if (!VulkanUtils::ValidationLayersEnabled()) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    VulkanUtils::FillDebugMessengerInfo(createInfo);

    VkLocalCheckF(VulkanUtils::CreateDebugUtilsMessengerEXT(m_Instance, &createInfo, NULL, &m_DebugUtils));
}

void GraphicsCore::CreateSurface(GraphicsCoreData& data)
{
    if (!SDL_Vulkan_CreateSurface(data.TargetWindow->GetSDLWindow(), m_Instance, &m_Surface)) {

    }
}

void GraphicsCore::CreateDeviceManager()
{
    m_DeviceManager = new DeviceManager();

    DeviceManagerData data{};
    data.Instance = m_Instance;
    data.Surface = m_Surface;
    data.Type = LOW_PERF_3D;

    m_DeviceManager->Initialize(&data);
}

void GraphicsCore::CreateRenderPass(GraphicsCoreData& _data)
{
    SwapChainSupportDetails swapChainSupport = SwapChainUtils::QuerySwapChainSupport(m_DeviceManager->GetPhysicalDevice(), m_Surface);
    VkSurfaceFormatKHR surfaceFormat = SwapChainUtils::ChooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = SwapChainUtils::ChooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = SwapChainUtils::ChooseSwapExtent(_data.TargetWindow, swapChainSupport.capabilities);

    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = surfaceFormat.format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    VkLocalCheckF(vkCreateRenderPass(m_DeviceManager->GetLogicalDevice(), &renderPassInfo, nullptr, &m_RenderPass));
}

void GraphicsCore::CreateSwapChainManager(GraphicsCoreData& _data)
{
    m_SwapChainManager = new SwapChainManager();

    SwapChainManagerData data{};
    data.Device = m_DeviceManager->GetLogicalDevice();
    data.PhysicalDevice = m_DeviceManager->GetPhysicalDevice();
    data.Surface = m_Surface;
    data.TargetWindow = _data.TargetWindow;
    data.RenderPass = m_RenderPass;

    m_SwapChainManager->Initialize(&data);
}

void GraphicsCore::CreateShaderManager(GraphicsCoreData& _data)
{
    m_ShaderManager = new ShaderManager();

    ShaderManagerData data{};
    data.WorkerCount = 2;
    data.AssetManager = _data.AssetManager;

    m_ShaderManager->Initialize(&data);
}

void GraphicsCore::CreateRendererCore(GraphicsCoreData& _data)
{
    m_RendererCore = new RendererCore();

    RendererCoreData data;
    data.Device = m_DeviceManager->GetLogicalDevice();
    data.GraphicsQueue = m_DeviceManager->GetGraphicsQueue();
    data.PhysicalDevice = m_DeviceManager->GetPhysicalDevice();
    data.PresentQueue = m_DeviceManager->GetPresentQueue();
    data.RenderPass = m_RenderPass;
    data.Surface = m_Surface;
    data.SwapChainManager = m_SwapChainManager;
    data.MaxFramesInFlight = 2;
    data.AssetManager = _data.AssetManager;

    m_RendererCore->Initialize(&data);
}

void GraphicsCore::CreateDebugRenderer(GraphicsCoreData& _data)
{
    m_DebugRenderer = new DebugRenderer();

    DebugRendererData data{};
    data.AssetManager = _data.AssetManager;
    data.Renderer = m_RendererCore;
    data.Shaders = m_ShaderManager;

    m_DebugRenderer->Initialize(&data);
}

void GraphicsCore::Initialize(InitializableBasic* data)
{
	GraphicsCoreData& mdata = *(GraphicsCoreData*)data;

	CreateInstance(mdata);
    CreateDebugMessenger();
    CreateSurface(mdata);
    CreateDeviceManager();
    CreateRenderPass(mdata);
    CreateSwapChainManager(mdata);
    CreateShaderManager(mdata);
    CreateRendererCore(mdata);
    if (mdata.InitializeDebugRenderer)
        CreateDebugRenderer(mdata);

    mR_Window = mdata.TargetWindow;
}

void GraphicsCore::Shutdown()
{
    m_RenderThreadEnding = true;
    (*mR_SyncVariable).notify_all();
    mR_Window->Expose();
    m_SwapChainManager->OnMinimizeEnd();

    vkDeviceWaitIdle(m_DeviceManager->GetLogicalDevice());

    if (m_DebugRenderer != NULL)
        m_DebugRenderer->Shutdown();

    m_RendererCore->Shutdown();

    m_ShaderManager->Shutdown();

    vkDestroyRenderPass(m_DeviceManager->GetLogicalDevice(), m_RenderPass, NULL);

    m_SwapChainManager->Shutdown();

    m_DeviceManager->Shutdown();

    VulkanUtils::DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugUtils, NULL);

    vkDestroySurfaceKHR(m_Instance, m_Surface, NULL);
    vkDestroyInstance(m_Instance, NULL);

    delete m_DeviceManager;
    delete m_SwapChainManager;
    delete m_ShaderManager;
    delete m_RendererCore;
}

void GraphicsCore::RunRenderingThread(std::mutex* syncMutex, std::condition_variable* syncVariable, bool* logicCompleted)
{
    mR_SyncMutex = syncMutex;
    mR_SyncVariable = syncVariable;
    mR_LogicThreadCompleted = logicCompleted;

    m_RenderThread = std::thread([this] {
        while (!m_RenderThreadEnding)
        {
            RenderingThreadCompleted = false;

            m_RendererCore->DrawFrame();

            if (m_DebugRenderer != NULL)
                m_DebugRenderer->Render();

            m_RendererCore->EndFrame();

            RenderingThreadCompleted = true;
            if (RenderingThreadCompleted && !(*mR_LogicThreadCompleted)) {
                std::unique_lock<std::mutex> lock(*mR_SyncMutex);
                (*mR_SyncVariable).wait(lock, [this] { return *mR_LogicThreadCompleted || m_RenderThreadEnding; });
            }
            else {
                (*mR_SyncVariable).notify_all();
            }
        }
    });

    m_RenderThread.detach();
}
