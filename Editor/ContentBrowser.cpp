#include "ContentBrowser.h"

void ContentBrowser::SearchTree(std::shared_ptr<CB_FileTreeElement> Parent, std::string Path, uint32_t& Id)
{
	for (auto entry : std::filesystem::directory_iterator(Path))
	{
		if (!std::filesystem::exists(entry.path()))
			continue;

		std::shared_ptr<CB_FileTreeElement> element = std::make_shared<CB_FileTreeElement>();
		element->HasParent = true;
		element->Id = Id;
		element->Name = entry.path().string().substr(entry.path().string().find_last_of("\\") + 1);

		if (std::filesystem::is_directory(entry.path()))
			SearchTree(element, entry.path().string(), Id);

		m_FileTreeElements.push_back(element);
		Parent->Children.push_back(element);

		Id++;
	}
}

bool ContentBrowser::SearchTreeElements(std::shared_ptr<CB_FileTreeElement> Parent)
{
	bool open = CustomNode((Parent->Name + "##" + std::to_string(Parent->Id)).c_str(), Parent->Children.size() == 0 ? ImGuiTreeNodeFlags_Bullet : 0);
	if (open) {
		for (size_t i = 0; i < Parent->Children.size(); i++)
			SearchTreeElements(Parent->Children[i]);
		ImGui::TreePop();
	}
	return open;
}

bool ContentBrowser::CustomNode(const char* label, ImGuiTreeNodeFlags flags) {
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	bool isSmall = (flags & ImGuiTreeNodeFlags_Bullet) != ImGuiTreeNodeFlags_Bullet;

	ImU32 id = window->GetID(label);
	ImVec2 pos = window->DC.CursorPos;
	ImRect bbFull(pos, ImVec2(pos.x + ImGui::GetContentRegionAvail().x, pos.y + g.FontSize + g.Style.FramePadding.y * 2));
	ImRect bbUse(pos, ImVec2(pos.x + g.FontSize + g.Style.FramePadding.y * 2, pos.y + g.FontSize + g.Style.FramePadding.y * 2));
	ImRect bb;
	if (!isSmall)
		bb = ImRect(bbUse.Min, ImVec2(pos.x + ImGui::GetContentRegionAvail().x, pos.y + g.FontSize + g.Style.FramePadding.y * 2));
	else
		bb = ImRect(bbUse.Max, ImVec2(pos.x + ImGui::GetContentRegionAvail().x, pos.y + g.FontSize + g.Style.FramePadding.y * 2));
	bool opened = ImGui::TreeNodeBehaviorIsOpen(id);
	bool hovered, held;
	if (isSmall) {
		if (ImGui::ButtonBehavior(bbUse, id, &hovered, &held, true))
			window->DC.StateStorage->SetInt(id, opened ? 0 : 1);
		if (hovered || held)
			window->DrawList->AddRectFilled(bbUse.Min, bbUse.Max, ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(held ? ImGuiCol_HeaderActive : ImGuiCol_HeaderHovered)));
	}

	// Icon, text
	float button_sz = g.FontSize + g.Style.FramePadding.y * 2;
	if (!isSmall) {
		window->DrawList->AddRectFilled(pos, ImVec2(pos.x + button_sz, pos.y + button_sz), ImColor(255, 255, 255));
		ImGui::RenderText(ImVec2(pos.x + button_sz + g.Style.ItemInnerSpacing.x, pos.y + g.Style.FramePadding.y), label);
	}
	else {
		ImGui::RenderArrow(window->DrawList, bbUse.Min + ImVec2(3.0f, 4.0f), 0xffffffff, opened ? ImGuiDir_Down : ImGuiDir_Up);
		ImGui::RenderText(ImVec2(pos.x + button_sz + g.Style.ItemInnerSpacing.x, pos.y + g.Style.FramePadding.y), label);
	}

	ImGui::ItemSize(bbFull, g.Style.FramePadding.y);
	ImGui::ItemAdd(bbFull, id);

	if (opened)
		ImGui::TreePush(label);
	return opened;
}

void ContentBrowser::Render(std::shared_ptr<ITextureManager> tm, std::string assets)
{
	if (ImGui::Begin("Content browser")) {
		float W = ImGui::GetContentRegionAvail().x;
		float WTree = W / 5.0f;
		float H = ImGui::GetContentRegionAvail().y;

		if (ImGui::BeginChild("##CB_FileTree", ImVec2(WTree, H), true)) {
			if (m_ReProcess <= 0.01f) {
				try
				{
					m_FileTreeElements.clear();

					std::shared_ptr<CB_FileTreeElement> Root = std::make_shared<CB_FileTreeElement>();
					Root->Name = "Assets";
					Root->Id = 0;

					m_FileTreeElements.push_back(Root);

					uint32_t Id = 1;
					SearchTree(Root, assets, Id);
				}
				catch (const std::exception& ex)
				{
					std::cout << ex.what() << std::endl;
					m_ReProcess = 0.01f;
				}
			}

			SearchTreeElements(m_FileTreeElements[0]);
		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGuiStyle style = ImGui::GetStyle();

		float PaddingW = style.WindowPadding.x;
		float ViewW = W - WTree - PaddingW; //Minus padding or it will get cutoff!
		if (ImGui::BeginChild("##CB_FileOverview", ImVec2(ViewW, H), true, ImGuiWindowFlags_MenuBar)) {
			float AvailableSpaceW = ImGui::GetContentRegionAvail().x;

			if (m_ItemSize > AvailableSpaceW / 3.0f)
				m_ItemSize = AvailableSpaceW / 3.0f;

			if (ImGui::BeginMenuBar()) {
				ImGui::SliderInt("##CB_FO_ItemSize", &m_ItemSize, 30, AvailableSpaceW / 3.0f);
			}
			ImGui::EndMenuBar();

			ImGuiContext& g = *ImGui::GetCurrentContext();
			ImDrawList* drawList = g.CurrentWindow->DrawList;

			bool isGrid = m_ItemSize > 30;
			int maxCharacters = (int)roundf(m_ItemSize / style.FramePadding.x * 2.0f - g.FontSize / 2.0f) / 2.0f - 2;

			uint32_t entryNum = 0;

			m_LastMaxSize = 0.0f;
			for (auto entry : std::filesystem::directory_iterator(assets + "\\" + m_FolderDepth))
			{
				std::string name = entry.path().string();
				name = name.substr(name.find_last_of("\\") + 1, maxCharacters);

				std::string extension = "File";

				if (std::filesystem::is_directory(entry.path())) {
					extension = "Folder";
				}
				else {
					uint32_t find_extension = name.find_last_of(".");
					if (find_extension != std::string::npos) {
						extension = name.substr(find_extension);
						name = name.substr(0, find_extension);
					}
				}

				TextureRef texture = NULL;

				if (extension == ".png" || extension == ".jpg" || extension == ".jpeg") {
					if (m_Textures.count(entry.path().string()))
						texture = m_Textures[entry.path().string()];
					else {
						auto output = tm->GetTexture(entry.path().string().substr(assets.size()+1));
						texture = output;

						m_Textures.insert(std::make_pair(entry.path().string(), texture));
					}
				}

				if (isGrid) {
					ImVec2 cPos = ImGui::GetCursorScreenPos();
					ImVec2 cLocalPos = ImGui::GetCursorPos();
					ImVec2 size = ImVec2(m_ItemSize, m_ItemSize + (g.FontSize + g.Style.FramePadding.y * 2)) + style.FramePadding * 2.0f;

					drawList->AddRectFilled(cPos, cPos + size, 0x30000000);

					if (texture == NULL)
						drawList->AddRectFilled(cPos + style.FramePadding, cPos + ImVec2(m_ItemSize, m_ItemSize) + style.FramePadding, 0xffffffff);
					else {
						auto tex_face = tm->GetTextureFromRef(texture);
						D11Texture2D* texD11 = (D11Texture2D*)tex_face.get();

						drawList->AddImage(texD11->GetResourceView(), cPos + style.FramePadding, cPos + ImVec2(m_ItemSize, m_ItemSize) + style.FramePadding);
					}
					
					float width = m_ItemSize;
					ImVec2 tSize = ImGui::CalcTextSize(name.c_str());
					ImGui::SetCursorScreenPos(cPos + ImVec2((width - tSize.x) / 2.0f, m_ItemSize) + style.FramePadding);
					ImGui::Text(name.c_str());

					if (ImGui::BeginItemTooltip()) {
						ImGui::SetTooltip(entry.path().string().c_str());
						ImGui::EndTooltip();
					}

					if (m_LastMaxSize < AvailableSpaceW - size.x * 2) {
						ImGui::SetCursorPos(cLocalPos + ImVec2(size.x, 0.0f));
						m_LastMaxSize += size.x;
					}
					else {
						m_LastMaxSize = 0.0f;
					}
				}
				else {
					if (entryNum % 2 == 0) {
						ImVec2 cPos = ImGui::GetCursorScreenPos(); //AddRectFilled is in screen coordinates so get cursor screen pos.
						drawList->AddRectFilled(cPos, cPos + ImVec2(AvailableSpaceW, g.FontSize + g.Style.FramePadding.y * 2), 0x20000000);	//Yes i did write the colors in hex
					}

					ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(5.0f, 3.0f));
					ImGui::Text(name.c_str());
					ImGui::SameLine();
					ImGui::SetCursorPosX(AvailableSpaceW - 50.0f);
					ImGui::Text(extension.c_str());
				}

				entryNum++;
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();

	if (m_ReProcess <= 0.0f)
		m_ReProcess = 10.0f; 
	m_ReProcess -= 0.016f;
}
