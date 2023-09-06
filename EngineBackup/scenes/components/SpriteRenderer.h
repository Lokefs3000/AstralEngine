#pragma once

#include "Variables.h"

#include "Component.h"

#include <glm/glm.hpp>
#include <graphics/managers/TextureRef.h>

struct SpriteRenderer : public Component
{
public:
	TextureRef Sprite = NULL;
	glm::vec4 Color = glm::vec4(1.0f);

	virtual uint16_t GetComponentId() override { return 3; };
	static uint16_t GetComponentId_S() { return 3; };
};