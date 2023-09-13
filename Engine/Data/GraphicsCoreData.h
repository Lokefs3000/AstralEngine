#pragma once

#include <string>

#include "Basics/Initializable.h"

class Window;

struct GraphicsCoreData : public InitializableBasic {
public:
	std::string ProductName;
	Window* TargetWindow;

	bool InitializeDebugRenderer;
};