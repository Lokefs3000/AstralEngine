#pragma once

#include "VarData.h"

#include <mutex>
#include <thread>

#include <vulkan/vulkan.h>

#include "Basics/Initializable.h"

struct GraphicsCoreData;

class DeviceManager;
class ShaderManager;
class SwapChainManager;
class RendererCore;

class GraphicsCore : public IInitializable {
private:
	VkInstance m_Instance;

	VkDebugUtilsMessengerEXT m_DebugUtils;

	VkSurfaceKHR m_Surface;

	VkRenderPass m_RenderPass;

	DeviceManager* m_DeviceManager;
	SwapChainManager* m_SwapChainManager;
	ShaderManager* m_ShaderManager;
	RendererCore* m_RendererCore;

	bool m_RenderThreadEnding;
	std::thread m_RenderThread;
	std::mutex* mR_SyncMutex;
	std::condition_variable* mR_SyncVariable;
	bool* mR_LogicThreadCompleted;

	void CreateInstance(GraphicsCoreData& data);
	void CreateDebugMessenger();
	void CreateSurface(GraphicsCoreData& data);
	void CreateDeviceManager();
	void CreateRenderPass(GraphicsCoreData& _data);
	void CreateSwapChainManager(GraphicsCoreData& data);
	void CreateShaderManager();
	void CreateRendererCore();
public:
	void EXPORT Initialize(InitializableBasic* data) override;
	void EXPORT Shutdown() override;

	bool RenderingThreadCompleted;
	void EXPORT RunRenderingThread(std::mutex* syncMutex, std::condition_variable* syncVariable, bool* logicCompleted);

	VkInstance GetInstance() { return m_Instance; }
	VkDebugUtilsMessengerEXT GetDebugMessenger() { return m_DebugUtils; }
	DeviceManager* GetDeviceManager() { return m_DeviceManager; }
	ShaderManager* GetShaderManager() { return m_ShaderManager; }
	SwapChainManager* GetSwapChainManager() { return m_SwapChainManager; }
	RendererCore* GetRendererCore() { return m_RendererCore; }
};