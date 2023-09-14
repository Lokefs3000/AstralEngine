#pragma once

#include "ASVarData.h"

#include <thread>
#include <vector>
#include <string>
#include <functional>
#include <mutex>

#include "WorkerQueue.h"
#include "ShaderWorkerData.h"

#define VERTEX_SHADER 0
#define FRAGMENT_SHADER 1

class ShaderWorker;

class ShaderCompiler {
private:
	std::vector<std::pair<std::thread, ShaderWorker*>> m_Workers;
	WorkerQueue<ShaderWorkerData> m_WorkerData;
public:
	ASEXPORT ShaderCompiler(uint8_t workerCount);
	ASEXPORT ~ShaderCompiler();

	void ASEXPORT AddShader(std::u16string vsource = u"", std::u16string fsource = u"", std::function<void(ShaderWorkerResult)> onCompleted = NULL);
};