#pragma once

#include "ImGui/imgui.h"

#include <debugging/Stopwatch.h>

#include <string>

class SceneManager;
class Scene;

class Explorer {
private:
	void DisplayTooltip(std::string content);

	void SceneDraw(std::shared_ptr<Scene> scene);
public:
	void Render(std::shared_ptr<SceneManager> sm);
};