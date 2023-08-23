#pragma once

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include <debugging/Stopwatch.h>

#include <string>

class SceneManager;
class Scene;

class Explorer {
private:
	void DisplayTooltip(std::string content);

	uint32_t m_SelectedSceneId;

	void SceneDraw(std::shared_ptr<Scene> scene, uint32_t sceneId);
public:
	void Render(std::shared_ptr<SceneManager> sm);
};