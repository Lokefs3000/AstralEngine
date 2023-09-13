#pragma once

#include "ASVarData.h"

#include <vector>
#include <mutex>

#include <glslang/SPIRV/GlslangToSpv.h>
#include <vulkan/vulkan.hpp>

#include "WorkerQueue.h"

struct ShaderWorkerData;
struct ShaderWorkerResult;

class ShaderWorker {
private:
	EShLanguage FindLanguage(vk::ShaderStageFlagBits shaderType);
	TBuiltInResource& CreateResource();

	ShaderWorkerResult& CompileData(ShaderWorkerData& data);

	WorkerQueue<ShaderWorkerData>& m_WorkerQueue;

	bool m_EndSignaled = false;
public:
	ShaderWorker(WorkerQueue<ShaderWorkerData>& queue);
	~ShaderWorker();

	void SignalEnd();
	void CompileShaders();
};