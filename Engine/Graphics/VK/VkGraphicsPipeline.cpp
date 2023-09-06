#include "pch.h"

#include "VkGraphicsPipeline.h"

#include <Data/Enum/GraphicsAPI.h>

void VkGraphicsPipeline::Initialize(ContextData& data)
{
}

void VkGraphicsPipeline::Shutdown()
{
}

void VkGraphicsPipeline::ClearFrame()
{
}

void VkGraphicsPipeline::PresentFrame()
{
}

GraphicsAPI VkGraphicsPipeline::GetGraphicsId()
{
    return GraphicsAPI::Vulkan;
}
