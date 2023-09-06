#pragma once

#include "Variables.h"

#include <cstdint>

struct Component {
public:
	virtual uint16_t GetComponentId() { return 0; };
	static uint16_t GetComponentId_S() { return 0; };
};