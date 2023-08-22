#pragma once

class Engine;

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include <debugging/Stopwatch.h>

#include <memory>

class PerformanceViewer {
private:
	bool m_IsGraph = false;

	bool activeWatches[6] = { true, true, true, true, true, true };

	float DrawBar(ImDrawList* dList, uint32_t color, StopWatch watch, float& x, float full, uint8_t id);

	void DrawTextLine(StopWatch watch, std::string name, uint32_t color, ImDrawList* dList, uint8_t id);
public:
	void Render(Engine* engine);
};