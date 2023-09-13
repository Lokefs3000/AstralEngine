#pragma once

#include <memory>
#include <vector>
#include <string>
#include <mutex>

class Window;
class GraphicsCore;
class EventHandler;
class AssetManager;

class EdEngine {
private:
	std::shared_ptr<Window> m_Window;
	std::shared_ptr<GraphicsCore> m_Graphics;
	std::shared_ptr<EventHandler> m_EventPoller;
	std::shared_ptr<AssetManager> m_EngineAssets;

	std::mutex m_SyncingMutex;
	std::condition_variable m_SyncingVariable;
	bool m_LogicCompleted;

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