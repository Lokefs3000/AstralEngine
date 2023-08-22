#include "Explorer.h"

#include "scenes/SceneManager.h"
#include "scenes/Scene.h"

void Explorer::DisplayTooltip(std::string content)
{
}

void Explorer::SceneDraw(std::shared_ptr<Scene> scene)
{
}

void Explorer::Render(std::shared_ptr<SceneManager> sm)
{
	if (ImGui::Begin("Explorer")) {
		if (ImGui::BeginChild("##ER_Scenes", ImGui::GetContentRegionAvail(), true)) {
			for (size_t i = 0; i < sm->GetScenes().size(); i++)
			{
				SceneDraw(sm->GetScenes()[i]);
			}

			ImGui::EndChild();
		}
	}
	ImGui::End();
}
