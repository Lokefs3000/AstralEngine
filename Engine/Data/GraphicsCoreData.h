#pragma once

#include <string>

#include "Basics/Initializable.h"

class Window;
class AssetManager;

struct GraphicsCoreData : public InitializableBasic {
public:
	std::string ProductName;
	Window* TargetWindow;
	AssetManager* AssetManager;

	bool InitializeDebugRenderer;
};