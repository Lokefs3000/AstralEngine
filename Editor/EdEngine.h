#pragma once

#include <memory>

#include <Debug/Logger.h>

#include "ProjectManager.h"
#include "ContentBrowser.h"

class Window;
__interface ITextureManager;
__interface IGraphicsContext;
__interface IAssetManager;

class EdEngine {
private:
	std::shared_ptr<Window> m_MainWindow;
	std::shared_ptr<IGraphicsContext> m_Context;
	std::shared_ptr<ITextureManager> m_Textures;

	std::shared_ptr<IAssetManager> m_AssetManager;
	std::shared_ptr<IAssetManager> m_ProjectAssetManager;

	std::unique_ptr<ProjectManager> m_ProjectManager;

	std::unique_ptr<ContentBrowser> m_ContentBrowser;

	float m_DeltaTime = 0.0f;

	void InitializeLoadWindow();
	void LoadProject(std::string dir);
public:
	EdEngine();
	~EdEngine();

	void Initialize(std::vector<std::string> Arguments);
	void Run();
	void Shutdown();
};