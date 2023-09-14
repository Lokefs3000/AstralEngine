#pragma once

#include "ASVarData.h"

#include <vector>
#include <mutex>
#include <string>

#include <glslang/SPIRV/GlslangToSpv.h>
#include <vulkan/vulkan.hpp>

#include "WorkerQueue.h"

struct ShaderWorkerData;
struct ShaderWorkerResult;

class ShaderWorker {
private:
	EShLanguage FindLanguage(vk::ShaderStageFlagBits shaderType);
	TBuiltInResource& CreateResource();

	std::vector<uint32_t> CompileData(std::u16string& source, vk::ShaderStageFlagBits bits);

	WorkerQueue<ShaderWorkerData>& m_WorkerQueue;

	bool m_EndSignaled = false;
public:
	ShaderWorker(WorkerQueue<ShaderWorkerData>& queue);
	~ShaderWorker();

	void SignalEnd();
	void CompileShaders();
};