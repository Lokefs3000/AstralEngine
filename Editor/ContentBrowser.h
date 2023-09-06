#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

struct TextureRef;
class GLTextureManager;
__interface ITexture;
__interface IAssetManager;
__interface ITextureManager;
class GLTexture2D;

struct CB_FileElement {
public:
	std::string Path = "";
	std::string Name = "";
	std::string Extension = "";

	uint32_t Id = 0;

	bool IsDirectory = false;

	std::vector<std::shared_ptr<CB_FileElement>> Elements;
};

class ContentBrowser {
private:
	std::string m_ProjectAssetFolder = "";
	std::string m_CurrentFolder = "";

	float m_SearchTime = 0.0f;
	std::shared_ptr<CB_FileElement> m_Root;
	std::map<std::string, std::shared_ptr<CB_FileElement>> m_DirElementMap;

	std::unique_ptr<GLTextureManager> m_TextureManager;
	std::shared_ptr<ITextureManager> m_Textures;

	std::shared_ptr<TextureRef> m_Tex_FolderClosed;
	std::shared_ptr<TextureRef> m_Tex_FolderOpened;
	std::shared_ptr<TextureRef> m_Tex_Model;
	std::shared_ptr<TextureRef> m_Tex_PlainFile;

	void SearchFileStructure(std::shared_ptr<CB_FileElement> Element, uint32_t& Id);

	void ShowFileTreeNodes(std::shared_ptr<CB_FileElement> Element);
	bool ShowFileTreeNode(std::shared_ptr<CB_FileElement> Element, GLTexture2D* opened, GLTexture2D* closed);
public:
	ContentBrowser(std::string assets, std::shared_ptr<IAssetManager> assetManager, std::shared_ptr<ITextureManager> textures);

	void Render(float dt);
};