#pragma once

#include "Variables.h"

#include "Component.h"

#include <glm/glm.hpp>

struct Transform : public Component
{
public:
	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Rotation = glm::vec3(0.0f);
	glm::vec3 Scale = glm::vec3(1.0f);

	virtual uint16_t GetComponentId() override { return 1; };
	static uint16_t GetComponentId_S() { return 1; };
};