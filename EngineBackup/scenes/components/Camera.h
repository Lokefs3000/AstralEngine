#pragma once

#include "Variables.h"

#include "Component.h"

#include <glm/glm.hpp>

struct Camera : public Component
{
public:
	float FieldOfView = 70.0f;
	float OrthographicSize = 0.0f;
	glm::vec3 ClearColor = glm::vec3(0.0f);

	virtual uint16_t GetComponentId() override { return 2; };
	static uint16_t GetComponentId_S() { return 2; };
};