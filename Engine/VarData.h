#pragma once

#include <vulkan/vulkan.h>

#ifdef ENGINE_EXPORTS
#define ALLOW_INCLUDE
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif

#define ONLY_DISCRETE 1
#define SUPPORT_GEOMETRY_SHADER 2

#define HIGH_PERF_3D ONLY_DISCRETE | SUPPORT_GEOMETRY_SHADER;
#define LOW_PERF_3D 0

#define USE_VULKAN_1_0 VK_API_VERSION_1_0;
#define USE_VULKAN_1_1 VK_API_VERSION_1_1;
#define USE_VULKAN_1_2 VK_API_VERSION_1_2;
#define USE_VULKAN_1_3 VK_API_VERSION_1_3;

#define BITWISE(bits, bit) (bits & bit) == bit