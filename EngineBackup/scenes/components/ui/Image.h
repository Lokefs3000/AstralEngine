#pragma once

#include "Variables.h"

#include <scenes/components/Component.h>

#include <glm/glm.hpp>
#include <graphics/managers/TextureRef.h>

struct Image : public Component
{
public:
	TextureRef Sprite = NULL;
	glm::vec4 Color = glm::vec4(1.0f);

	virtual uint16_t GetComponentId() override { return 5; };
	static uint16_t GetComponentId_S() { return 5; };
};