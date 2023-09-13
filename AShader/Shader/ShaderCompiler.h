#pragma once

#include "ASVarData.h"

#include <thread>
#include <vector>
#include <string>
#include <functional>
#include <mutex>

#define VERTEX_SHADER 0
#define FRAGMENT_SHADER 1

class ShaderWorker;
struct ShaderWorkerData;
struct ShaderWorkerResult;

class ShaderCompiler {
private:
	std::vector<std::pair<std::thread, ShaderWorker*>> m_Workers;
	std::vector<ShaderWorkerData> m_WorkerData;

	std::mutex m_WorkerDataMutex;
	std::condition_variable m_WorkerDataCondition;
public:
	ASEXPORT ShaderCompiler(uint8_t workerCount);
	ASEXPORT ~ShaderCompiler();

	void ASEXPORT AddShader(std::u16string source = u"", uint8_t identifier = VERTEX_SHADER, std::function<void(ShaderWorkerResult)> onCompleted = NULL);
};