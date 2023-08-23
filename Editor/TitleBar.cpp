#include "TitleBar.h"

#include "scenes/SceneManager.h"

void TitleBar::DisplayTooltip(std::string content)
{
	if (ImGui::BeginItemTooltip()) {
		ImGui::Text(content.c_str());
		ImGui::EndTooltip();
	}
}

void TitleBar::Render(StopWatch deltaWatch, std::shared_ptr<SceneManager> sm)
{
	float dt = deltaWatch->GetDuration() / 1000.0f;
	uint32_t fps = (uint32_t)roundf(1.0f / dt);

	if (ImGui::BeginMainMenuBar()) {
		float beginX = ImGui::GetCursorPosX();
		float availW = ImGui::GetContentRegionAvail().x;

		if (ImGui::BeginMenu("File")) {
			if (ImGui::BeginMenu("New")) {
				if (ImGui::MenuItem("New standalone")) {
					sm->LoadGlobalScene("");
				}
				if (ImGui::MenuItem("New parallel")) {
					sm->LoadScene("");
				}

				ImGui::EndMenu();
			}
			DisplayTooltip("Create a new level.");

			ImGui::Separator();

			if (ImGui::MenuItem("Return")) {

			}
			DisplayTooltip("Return to the project selection screen.");

			if (ImGui::MenuItem("Exit")) {

			}
			DisplayTooltip("Exit the application to the desktop.");

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit")) {
			ImGui::Separator();

			if (ImGui::MenuItem("Project options")) {

			}
			DisplayTooltip("Change the project options.");

			if (ImGui::MenuItem("Editor options")) {

			}
			DisplayTooltip("Change the editor options.");

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View")) {
			ImGui::EndMenu();
		}

		std::string fpsText = "Framerate: " + std::to_string(fps) + "  Memory: 0mb";
		ImGui::SetCursorPosX(beginX + availW - ImGui::CalcTextSize(fpsText.c_str()).x);
		ImGui::Text(fpsText.c_str());

		ImGui::EndMainMenuBar();
	}
}
