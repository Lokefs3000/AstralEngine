#pragma once

#include "ImGui/imgui.h"

#include <debugging/Stopwatch.h>

#include <string>

class SceneManager;

class TitleBar {
private:
	void DisplayTooltip(std::string content);
public:
	void Render(StopWatch deltaWatch, std::shared_ptr<SceneManager> sm);
};