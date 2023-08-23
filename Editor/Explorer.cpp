#include "Explorer.h"

#include "scenes/SceneManager.h"
#include "scenes/Scene.h"

void Explorer::DisplayTooltip(std::string content)
{
}

void Explorer::SceneDraw(std::shared_ptr<Scene> scene, uint32_t sceneId)
{
	ImDrawList* dList = ImGui::GetWindowDrawList();
	ImGuiContext& ctx = *ImGui::GetCurrentContext();

	ImVec2 sC = ImGui::GetCursorScreenPos();
	ImVec2 cRA = ImGui::GetContentRegionAvail();

	dList->AddRectFilled(sC, sC + ImVec2(cRA.x, 25.0f), 0x10ffffff);

	std::string name = scene->Name.empty() ? "Unsaved scene" : scene->Name;
	ImVec2 tS = ImGui::CalcTextSize(name.c_str());
	ImVec2 lC = ImGui::GetCursorPos();

	ImVec2 tPos = ImVec2(cRA.x, 25.0f) - tS;
	ImGui::SetCursorPos(lC + tPos / 2.0f);
	ImGui::Text(name.c_str());

	ImVec2 arrPos = sC + ImVec2(7.0f, tPos.y / 2.0f);

	if (ImGui::IsMouseHoveringRect(arrPos, arrPos + ImVec2(ctx.FontSize, ctx.FontSize)))
		dList->AddRectFilled(arrPos, arrPos + ImVec2(ctx.FontSize, ctx.FontSize), ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGui::IsMouseDown(0) ? ImGuiCol_HeaderActive : ImGuiCol_HeaderHovered)));
	ImGui::RenderArrow(dList, arrPos, 0xffffffff, ImGuiDir_Down);

	if (ImGui::IsMouseHoveringRect(sC, sC + ImVec2(cRA.x, cRA.y))) {
		m_SelectedSceneId = sceneId;
	}

	for (size_t i = 0; i < scene->GetEntities().size(); i++)
	{
		auto component = 
		if (ImGui::TreeNode())
	}
}

void Explorer::Render(std::shared_ptr<SceneManager> sm)
{
	if (ImGui::Begin("Explorer")) {
		if (ImGui::BeginChild("##ER_Scenes", ImGui::GetContentRegionAvail(), true)) {
			m_SelectedSceneId = 0;

			for (size_t i = 0; i < sm->GetScenes().size(); i++)
			{
				SceneDraw(sm->GetScenes()[i], i);
			}

			if (ImGui::BeginPopupContextWindow("##ER_Scenes_Context")) {
				if (ImGui::MenuItem("New Entity")) {
					if (m_SelectedSceneId < sm->GetScenes().size()) {
						sm->GetScenes()[m_SelectedSceneId]->AddEntity();
					}
				}
				DisplayTooltip("Create a new entity in this scene.");

				ImGui::EndPopup();
			}

			ImGui::EndChild();
		}
	}
	ImGui::End();
}
