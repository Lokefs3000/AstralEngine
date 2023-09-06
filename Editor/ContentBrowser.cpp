#include "ContentBrowser.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include <Graphics/GL/Managers/GLTextureManager.h>
#include <Graphics/GL/Resources/GLTexture2D.h>

#include <Basics/AssetManager.h>

#include <Data/TextureManagerData.h>

#include <filesystem>
#include <iostream>

void ContentBrowser::SearchFileStructure(std::shared_ptr<CB_FileElement> Element, uint32_t& Id)
{
	Id++;
	for (auto files : std::filesystem::directory_iterator(Element->Path))
	{
		std::shared_ptr<CB_FileElement> File = std::make_shared<CB_FileElement>();
		File->Path = files.path().string();
		File->Name = File->Path.substr(File->Path.find_last_of("\\")+1);
		File->Extension = files.path().has_extension() ? files.path().extension().string().substr(1) : "";
		File->Id = Id;

		Element->Elements.push_back(File);
		
		if (std::filesystem::is_directory(File->Path)) {
			File->IsDirectory = true;
			m_DirElementMap.insert(std::make_pair(File->Path + "\\", File));
			SearchFileStructure(File, Id);
		}
	}
}

void ContentBrowser::ShowFileTreeNodes(std::shared_ptr<CB_FileElement> Element)
{
	GLTexture2D* opened = (GLTexture2D*)m_Textures->FromRef(m_Tex_FolderOpened).get();
	GLTexture2D* closed = (GLTexture2D*)m_Textures->FromRef(m_Tex_FolderClosed).get();

	if (Element.get() != NULL) {
		if (Element->Extension == "png" || Element->Extension == "jpg" || Element->Extension == "jpeg") {
			opened = (GLTexture2D*)m_TextureManager->FromRef(m_TextureManager->GetTexture2D(Element->Path.substr(m_ProjectAssetFolder.size()))).get();
			closed = opened;
		}
		else if (Element->IsDirectory) {
			opened = (GLTexture2D*)m_Textures->FromRef(m_Tex_FolderOpened).get();
			closed = (GLTexture2D*)m_Textures->FromRef(m_Tex_FolderClosed).get();
		}
	}

	if (ShowFileTreeNode(Element, opened, closed)) {
		for (size_t i = 0; i < Element->Elements.size(); i++)
			ShowFileTreeNodes(Element->Elements[i]);

		ImGui::TreePop();
	}
}

bool ContentBrowser::ShowFileTreeNode(std::shared_ptr<CB_FileElement> Element, GLTexture2D* t_opened, GLTexture2D* t_closed)
{
	if (Element.get() == NULL) {
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "CB_FileElement tree pointer is null!");
		return false;
	}

	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	std::string nameId = Element->Name + "##CB_FT_" + std::to_string(Element->Id);
	const char* nameIdc = nameId.c_str();

	std::string nameId2 = Element->Name + "##CB_FT_BP_" + std::to_string(Element->Id);
	const char* nameIdc2 = nameId2.c_str();

	ImU32 id = window->GetID(nameIdc);
	ImU32 id2 = window->GetID(nameIdc2);
	ImVec2 pos = window->DC.CursorPos;
	float button_sz = g.FontSize + g.Style.FramePadding.y * 2;
	ImRect bb(pos, ImVec2(pos.x + ImGui::GetContentRegionAvail().x, pos.y + g.FontSize + g.Style.FramePadding.y * 2));
	ImRect bb2(pos, ImVec2(pos.x + g.FontSize + g.Style.FramePadding.y * 2, pos.y + g.FontSize + g.Style.FramePadding.y * 2));
	ImRect bb3(ImVec2(bb2.Max.x + g.Style.FramePadding.x, pos.y), ImVec2(pos.x + ImGui::GetContentRegionAvail().x, pos.y + g.FontSize + g.Style.FramePadding.y * 2));
	bool opened = ImGui::TreeNodeBehaviorIsOpen(id);
	bool hovered, held;
	bool hovered2, held2;
	if (ImGui::ButtonBehavior(bb2, id, &hovered, &held, true))
		window->DC.StateStorage->SetInt(id, opened ? 0 : 1);
	if (ImGui::ButtonBehavior(bb3, id2, &hovered2, &held2, true)) {
		if (Element->IsDirectory) {
			m_CurrentFolder = Element->Path.substr(m_ProjectAssetFolder.size()) + "\\";
		}
	}
	if ((hovered || held) && Element->IsDirectory)
		window->DrawList->AddRectFilled(bb2.Min, bb2.Max, ImGui::ColorConvertFloat4ToU32(g.Style.Colors[held ? ImGuiCol_HeaderActive : ImGuiCol_HeaderHovered]));
	if (hovered2 || held2)
		window->DrawList->AddRectFilled(bb3.Min, bb3.Max, ImGui::ColorConvertFloat4ToU32(g.Style.Colors[held ? ImGuiCol_HeaderActive : ImGuiCol_HeaderHovered]));

	// Icon, text
	ImVec2 pos2 = pos + ImVec2(button_sz + g.Style.FramePadding.x, 0.0f);
	window->DrawList->AddImage((ImTextureID)(opened ? t_opened->GetTextureId() : t_closed->GetTextureId()), pos2, ImVec2(pos2.x + button_sz, pos2.y + button_sz));
	ImGui::RenderText(ImVec2(pos2.x + button_sz + g.Style.ItemInnerSpacing.x, pos2.y + g.Style.FramePadding.y), Element->Name.c_str());

	if (Element->IsDirectory)
		ImGui::RenderArrow(window->DrawList, pos + ImVec2(2.5f, 2.5f), 0xffffffff, opened ? ImGuiDir_Down : ImGuiDir_Right);

	ImGui::ItemSize(bb, g.Style.FramePadding.y);
	ImGui::ItemAdd(bb2, id);
	ImGui::ItemAdd(bb3, id2);

	if (opened)
		ImGui::TreePush(Element->Name.c_str());
	return opened;
}

ContentBrowser::ContentBrowser(std::string assets, std::shared_ptr<IAssetManager> assetManager, std::shared_ptr<ITextureManager> textures)
{
	m_ProjectAssetFolder = assets;

	m_TextureManager = std::make_unique<GLTextureManager>();
	m_TextureManager->Initialize(assetManager);

	m_Textures = textures;

	m_Tex_FolderClosed = m_Textures->GetTexture2D("textures\\folder_closed.png");
	m_Tex_FolderOpened = m_Textures->GetTexture2D("textures\\folder_opened.png");
	m_Tex_Model = m_Textures->GetTexture2D("textures\\model.png");
	m_Tex_PlainFile = m_Textures->GetTexture2D("textures\\file_plain.png");
}

void ContentBrowser::Render(float dt)
{
	m_SearchTime -= dt;
	if (m_SearchTime < 0.0f) {
		m_SearchTime = 3.0f;

		m_DirElementMap.clear();

		m_Root = std::make_shared<CB_FileElement>();
		m_Root->Path = m_ProjectAssetFolder;
		m_Root->Name = "Assets";
		m_Root->Id = 0;
		m_Root->IsDirectory = true;

		uint32_t id = 0;
		SearchFileStructure(m_Root, id);
	}

	if (ImGui::Begin("Content browser")) {
		ImVec2 availSize = ImGui::GetContentRegionAvail();
		float sideWidth = availSize.x / 5.0f;

		ImGuiContext& ctx = *ImGui::GetCurrentContext();
		ImGuiStyle& style = ctx.Style;

		if (ImGui::BeginChild("##CB_FileTree", ImVec2(sideWidth, availSize.y), true)) {
			ShowFileTreeNodes(m_Root);
		}
		ImGui::EndChild();

		ImGui::SameLine();

		if (ImGui::BeginChild("##CB_FileView", ImVec2(availSize.x - sideWidth - style.WindowPadding.x, availSize.y), true)) {
			std::string currentFolder = m_ProjectAssetFolder + m_CurrentFolder;

			if (!std::filesystem::exists(currentFolder) || !std::filesystem::is_directory(currentFolder))
				m_CurrentFolder = "";

			std::shared_ptr<CB_FileElement> Element = m_DirElementMap.count(currentFolder) ? m_DirElementMap.at(currentFolder) : m_Root;
			
			if (Element.get() != NULL) {
				ImVec2 availSize = ImGui::GetContentRegionAvail();
				float localXStart = ImGui::GetCursorPosX();

				ImGuiContext& ctx = *ImGui::GetCurrentContext();
				ImGuiStyle& style = ctx.Style;
				ImDrawList* dList = ctx.CurrentWindow->DrawList;

				float y = ImGui::GetCursorPosY();

				for (size_t i = 0; i < Element->Elements.size(); i++)
				{
					std::shared_ptr<CB_FileElement> elem = Element->Elements[i];

					uint8_t fileType = 0;

					if (elem->Extension == "png" || elem->Extension == "jpg" || elem->Extension == "jpeg") {
						fileType = 1;
					} else if (elem->IsDirectory) {
						fileType = 2;
					}

					float localXCurrent = ImGui::GetCursorPosX();

					if (localXCurrent + 80.0f + style.FramePadding.x > availSize.x) {
						y += ImGui::GetCursorPosY() + 95.0f;
						ImGui::SetCursorPos(ImVec2(localXStart, y));
						localXCurrent = localXStart;
					}
					else
						ImGui::SetCursorPosY(y);

					ImVec2 cSP = ImGui::GetCursorScreenPos();

					std::string sNameId = elem->Name + "##CB_FV_" + std::to_string(elem->Id);
					const char* nameDisp = elem->Name.c_str();
					const char* nameId = sNameId.c_str();

					ImGuiID id = ImGui::GetID(nameId);

					ImRect fullSize = ImRect(cSP, cSP + ImVec2(80.0f, 95.0f));

					bool hovered, held;
					if (ImGui::ButtonBehavior(fullSize, id, &hovered, &held, ImGuiButtonFlags_PressedOnDoubleClick)) {
						if (std::filesystem::is_directory(elem->Path)) {
							m_CurrentFolder += elem->Name + "\\";
						}
					}

					uint32_t colorId = 0x10ffffff;
					if (held) colorId += 0x20ffffff;
					else if (hovered) colorId += 0x10ffffff;

					GLTexture2D* texture = (GLTexture2D*)m_Textures->FromRef(m_Tex_PlainFile).get();
					if (fileType == 1) {
						texture = (GLTexture2D*)m_TextureManager->FromRef(m_TextureManager->GetTexture2D(elem->Path.substr(m_ProjectAssetFolder.size()))).get();
					} else if (fileType == 2) {
						texture = (GLTexture2D*)m_Textures->FromRef(m_Tex_FolderClosed).get();
					}

					dList->AddRectFilled(fullSize.Min, fullSize.Max, colorId, 4.0f);
					
					if (texture == NULL)
						dList->AddRectFilled(cSP + style.FramePadding * 2, cSP + ImVec2(80.0f, 80.0f) - style.FramePadding * 2, 0xffffffff, 4.0f);
					else
						dList->AddImageRounded((ImTextureID)texture->GetTextureId(), cSP + style.FramePadding * 2, cSP + ImVec2(80.0f, 80.0f) - style.FramePadding * 2, ImVec2(0, 0), ImVec2(1, 1), 0xffffffff, 4.0f);

					float textWidth = ImGui::CalcTextSize(nameDisp).x;
					dList->AddText(cSP + ImVec2(40.0f - textWidth / 2.0f, 80.0f), 0xffffffff, nameDisp);

					ImGui::ItemSize(fullSize);
					ImGui::ItemAdd(fullSize, id);

					ImGui::SameLine();

					ImGui::SetCursorPosX(localXCurrent + 80.0f + style.FramePadding.x);
				}
			}
			else {
				ImGui::TextColored(ImVec4(1, 0, 0, 1), "CB_FileElement pointer is null!");
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();

	//m_TextureManager->UpdateClearTimer(dt);
}
