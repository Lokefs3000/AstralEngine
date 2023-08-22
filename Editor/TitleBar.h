#pragma once

#include "ImGui/imgui.h"

#include <debugging/Stopwatch.h>

#include <string>

class TitleBar {
private:
	void DisplayTooltip(std::string content);
public:
	void Render(StopWatch deltaWatch);
};