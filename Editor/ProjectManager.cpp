#include "ProjectManager.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"
#include "ImGui/imgui_internal.h"

#include <filesystem>
#include <fstream>
#include <sstream>

#include <Data/AstralData.h>
#include <Format/AstralLoader.h>
#include <Debug/Logger.h>

ProjectManager::ProjectManager()
{
	std::ifstream projsFile("editor\\assets\\config\\projects.astral");
	std::stringstream projsStrS;
	projsStrS << projsFile.rdbuf();
	projsFile.close();

	m_Projects = AstralLoader::LoadAstral(projsStrS.str());
}

void ProjectManager::Render(std::function<void(std::string)> func)
{
	if (ImGui::Begin("Project manager")) {
		ImVec2 availSpace = ImGui::GetContentRegionAvail();
		float menuHeight = 35.0f;

		ImGuiStyle& style = ImGui::GetStyle();
		float padding = style.WindowPadding.y;

		if (ImGui::BeginChild("##PM_Projects", availSpace - ImVec2(0.0f, menuHeight + padding), true)) {
			ImGuiContext& ctx = *ImGui::GetCurrentContext();
			ImDrawList* dList = ctx.CurrentWindow->DrawList;
			ImGuiStyle& style = ctx.Style;

			ImVec2 totalSize = ImGui::GetContentRegionAvail();

			auto saved = m_Projects->GetChild("Projects")->GetValue("Saved");
			for (size_t i = 0; i < saved->GetSize(); i++)
			{
				auto at = saved->GetAt<std::string>(i);
				
				ImVec2 cSP = ImGui::GetCursorScreenPos();

				dList->AddRectFilled(cSP, cSP + ImVec2(totalSize.x, 14.0f + style.WindowPadding.y * 2), 0x10ffffff, 4.0f);
			
				ImVec2 cP = ImGui::GetCursorPos();
				ImGui::SetCursorPos(cP + style.WindowPadding);
				ImGui::Text(at.c_str());

				ImGui::SetCursorPos(cP + style.WindowPadding + ImVec2(totalSize.x - 54.0f, -2.5f));
				if (ImGui::Button(("Open##" + at + std::to_string(i)).c_str())) {
					func(at);
				}
				ImGui::NewLine();
			}
		}
		ImGui::EndChild();

		if (ImGui::BeginChild("##PM_Controls", ImVec2(availSpace.x, menuHeight), true)) {
			if (ImGui::Button("Open")) {

			}
			ImGui::SameLine();
			if (ImGui::Button("New")) {
				m_NewProjectVisible = true;
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();

	if (m_NewProjectVisible)
		ImGui::OpenPopup("New project##PM_NewProject");
	if (ImGui::BeginPopupModal("New project##PM_NewProject", &m_NewProjectVisible, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::InputText("##PM_NP_ProjectName", &m_ProjectName);

		bool canCreate = true;

		if (m_ProjectName.empty()) {
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "Project name cannot be empty");
			canCreate = false;
		} else {
			bool exists = false;

			for (auto entry : std::filesystem::directory_iterator("projects\\"))
			{
				std::string path = entry.path().string().substr(9);
				if (path == m_ProjectName) {
					exists = true;
					canCreate = false;
					break;
				}
			}

			if (exists)
				ImGui::TextColored(ImVec4(1, 0, 0, 1), "Project of name already exists");
			else
				ImGui::NewLine();
		}

		if (ImGui::Button("Cancel")) {
			m_NewProjectVisible = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (!canCreate)
			ImGui::BeginDisabled();
		if (ImGui::Button("Create")) {
			if (canCreate) {
				std::filesystem::create_directory("projects\\" + m_ProjectName);
				std::filesystem::create_directory("projects\\" + m_ProjectName + "\\assets");
				std::filesystem::create_directory("projects\\" + m_ProjectName + "\\data");

				std::fstream writeStream("projects\\" + m_ProjectName + "\\project.astral", std::ios::out);
				writeStream << "";
				writeStream.close();

				std::string projectConfig = "Projects: {\n";
				projectConfig += "	Saved: [\n";

				auto saved = m_Projects->GetChild("Projects")->GetValue("Saved");
				for (size_t i = 0; i < saved->GetSize(); i++)
				{
					auto at = saved->GetAt<std::string>(i);
					projectConfig += "		" + at + "\n";
				}
				projectConfig += "		" + m_ProjectName + "\n";

				projectConfig += "	]\n";
				projectConfig += "}";

				std::fstream pConfigStream("editor\\assets\\config\\projects.astral", std::ios::out);
				pConfigStream << projectConfig;
				pConfigStream.close();

				m_Projects = AstralLoader::LoadAstral(projectConfig);

				func(m_ProjectName);

				m_NewProjectVisible = false;
				ImGui::CloseCurrentPopup();
			}
		}
		if (!canCreate)
			ImGui::EndDisabled();

		ImGui::EndPopup();
	}
}
