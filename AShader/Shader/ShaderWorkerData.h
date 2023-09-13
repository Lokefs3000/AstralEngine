#pragma once

#include <string>
#include <functional>
#include <vulkan/vulkan.hpp>

typedef std::vector<uint32_t> ShaderBytecode;

struct ShaderWorkerResult
{
public:
	ShaderBytecode Bytecode;
};

struct ShaderWorkerData
{
public:
	std::u16string ShaderSource;

	vk::ShaderStageFlagBits StageBits;

	std::function<void(ShaderWorkerResult& result)> OnCompleted;
};