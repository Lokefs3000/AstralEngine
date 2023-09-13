#pragma once

#include <string>

#include "Basics/Initializable.h"

struct WindowData : public InitializableBasic {
public:
	std::string Title;
	uint32_t Width;
	uint32_t Height;
};