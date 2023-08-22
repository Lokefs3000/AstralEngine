#pragma once

#include "../Variables.h"

#include "EngineLayer.h"
#include "loader/Configurations.h"

#ifdef EXPOSE_ENGINE
#include <type_traits>
#include "utils/ApiUtils.h"
#endif

#include "debugging/Stopwatch.h"

class Window;
class SceneManager;
__interface IGraphicsContext;
__interface IRenderer;
__interface IAssetManager;
__interface ITextureManager;

#include <vector>
#include <memory>

class EXPORT Engine {
private:
	std::shared_ptr<Window> m_MainWindow;

	std::shared_ptr<IGraphicsContext> m_GraphicsContext;
	std::shared_ptr<IRenderer> m_Renderer;

	std::shared_ptr<IAssetManager> m_AssetManager;

	std::shared_ptr<ITextureManager> m_TextureManager;

	std::shared_ptr<SceneManager> m_SceneManager;

	std::string m_ProjectFolder = "";
public:
	Engine();
	~Engine();

	struct StopWatches
	{
		StopWatch DeltaWatch;
		StopWatch RenderWatch;
		StopWatch AssetWatch;
		StopWatch TextureWatch;
		StopWatch LayerWatch;
		StopWatch LateLayerWatch;
		StopWatch EventWatch;
	};
	StopWatches Watches;

	std::vector<std::shared_ptr<EngineLayer>> EngineLayers = std::vector<std::shared_ptr<EngineLayer>>();
	std::shared_ptr<ConfigObject> ProjectConfig;

	template<typename T>
	std::shared_ptr<T> AddEngineLayer();

	void LoadProjectConfig(std::string config);

	std::shared_ptr<Window> GetWindow() { return m_MainWindow; };
	std::shared_ptr<IGraphicsContext> GetContext() { return m_GraphicsContext; }
	std::shared_ptr<IRenderer> GetRenderer() { return m_Renderer; }
	std::shared_ptr<IAssetManager> GetAssetManager() { return m_AssetManager; }
	std::shared_ptr<ITextureManager> GetTextureManager() { return m_TextureManager; }
	std::shared_ptr<SceneManager> GetSceneManager() { return m_SceneManager; }

	void Initialize();
	void Run();
	void Shutdown();

	DEFINED_AS
};

template<typename T>
inline std::shared_ptr<T> Engine::AddEngineLayer()
{
	static_assert(std::is_base_of_v<EngineLayer, T>, "T is not a child of IEngineLayer");

	std::shared_ptr<T> layer = std::make_shared<T>(this);
	EngineLayers.push_back(layer);

	return layer;
}