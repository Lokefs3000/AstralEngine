#pragma once

#include "Variables.h"

#include "Component.h"

#include <glm/glm.hpp>

enum class CanvasScaling {
	None,
	Scale,
	ScaleUniform
};

struct Canvas : public Component
{
public:
	CanvasScaling Scaling = CanvasScaling::None;
	glm::ivec2 Size = glm::ivec2(1280.0f, 826.0f);

	virtual uint16_t GetComponentId() override { return 4; };
	static uint16_t GetComponentId_S() { return 4; };
};