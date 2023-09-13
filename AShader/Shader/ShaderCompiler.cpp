#include "pch.h"
#include "ShaderCompiler.h"

#include "ShaderWorkerData.h"
#include "ShaderWorker.h"

ShaderCompiler::ShaderCompiler(uint8_t threadCount)
{
	m_Workers.resize(threadCount);

	for (size_t i = 0; i < threadCount; i++)
	{
		ShaderWorker* sworker = new ShaderWorker(m_WorkerDataMutex, m_WorkerDataCondition, m_WorkerData);
		m_Workers[i] = std::make_pair(std::thread(&ShaderWorker::CompileShaders, sworker), sworker);
		m_Workers[i].first.detach();
	}
}

ShaderCompiler::~ShaderCompiler()
{
	for (size_t i = 0; i < m_Workers.size(); i++)
	{
		m_Workers[i].second->SignalEnd();
		m_Workers[i].first.join();
		delete m_Workers[i].second;
	}

	m_Workers.clear();
}

void ShaderCompiler::AddShader(std::u16string source, uint8_t identifier, std::function<void(ShaderWorkerResult)> onCompleted)
{
	ShaderWorkerData data;
	data.ShaderSource = source;
	data.OnCompleted = onCompleted;

	switch (identifier)
	{
	case 0:
		data.StageBits = vk::ShaderStageFlagBits::eVertex;
		break;
	case 1:
		data.StageBits = vk::ShaderStageFlagBits::eFragment;
		break;
	default:
		data.StageBits = vk::ShaderStageFlagBits::eVertex;
		break;
	}

	std::lock_guard lock(m_WorkerDataMutex);

	m_WorkerData.push_back(data);

	m_WorkerDataCondition.notify_one();
}
