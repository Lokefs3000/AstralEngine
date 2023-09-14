#pragma once

#include <string>
#include <functional>
#include <vulkan/vulkan.hpp>

typedef std::vector<uint32_t> ShaderBytecode;

struct ShaderWorkerResult
{
public:
	ShaderBytecode VBytecode;
	ShaderBytecode FBytecode;
};

struct ShaderWorkerData
{
public:
	std::u16string VShaderSource;
	std::u16string FShaderSource;

	std::function<void(ShaderWorkerResult& result)> OnCompleted;
};