#pragma once

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include <iostream>
#include <string>
#include <filesystem>

struct CB_FileTreeElement {
	uint32_t Id;
	std::vector<std::shared_ptr<CB_FileTreeElement>> Children;
	bool HasParent = false;
	std::string Name;
};

class ContentBrowser {
private:
	std::vector<std::shared_ptr<CB_FileTreeElement>> m_FileTreeElements;
	void SearchTree(std::shared_ptr<CB_FileTreeElement> Parent, std::string Path, uint32_t& Id);
	bool SearchTreeElements(std::shared_ptr<CB_FileTreeElement> Parent);
	bool CustomNode(const char* label, ImGuiTreeNodeFlags flags);

	int m_ItemSize = 55; //TODO: tweak maybe
	float m_LastMaxSize = 9999999.0f; //Big

	std::string m_FolderDepth = "";
public:
	void Render(std::string assets);
};