#pragma once

#include "Variables.h"

#include "Component.h"

#include <glm/glm.hpp>

struct Transform : public Component
{
	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Rotation = glm::vec3(0.0f);
	glm::vec3 Scale = glm::vec3(1.0f);

	static uint16_t GetComponentId() { return 1; };
};