#pragma once

#include "Basics/GraphicsContext.h"

#include <vulkan/vulkan.h>

class VkGraphicsPipeline : public IGraphicsContext {
private:
	
public:
	void Initialize(ContextData& data) override;
	void Shutdown() override;

	void ClearFrame() override;
	void PresentFrame() override;

	GraphicsAPI GetGraphicsId() override;
};