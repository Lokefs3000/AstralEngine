#pragma once

#include "ASVarData.h"

#include <vector>
#include <mutex>

#include <glslang/SPIRV/GlslangToSpv.h>
#include <vulkan/vulkan.hpp>

struct ShaderWorkerData;
struct ShaderWorkerResult;

class ShaderWorker {
private:
	EShLanguage FindLanguage(vk::ShaderStageFlagBits shaderType);
	TBuiltInResource& CreateResource();

	ShaderWorkerResult& CompileData(ShaderWorkerData& data);

	std::mutex& m_Mutex;
	std::condition_variable& m_Condition;
	std::vector<ShaderWorkerData>& m_Data;

	bool m_EndSignaled = false;
public:
	ShaderWorker(std::mutex& mutex, std::condition_variable& condition, std::vector<ShaderWorkerData>& data);
	~ShaderWorker();

	void SignalEnd();
	void CompileShaders();
};