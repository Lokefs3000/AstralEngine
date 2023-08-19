#pragma once

#include "../Variables.h"

#include "EngineLayer.h"
#include "loader/Configurations.h"

#ifdef EXPOSE_ENGINE
#include <type_traits>
#include "utils/ApiUtils.h"
#endif

class Window;
__interface IGraphicsContext;
__interface IRenderer;

#include <vector>
#include <memory>

class EXPORT Engine {
private:
	std::shared_ptr<Window> m_MainWindow;

	std::shared_ptr<IGraphicsContext> m_GraphicsContext;
	std::shared_ptr<IRenderer> m_Renderer;
public:
	Engine();
	~Engine();

	std::vector<std::shared_ptr<EngineLayer>> EngineLayers = std::vector<std::shared_ptr<EngineLayer>>();
	std::shared_ptr<ConfigObject> ProjectConfig;

	template<typename T>
	std::shared_ptr<T> AddEngineLayer();

	void LoadProjectConfig(std::string config);

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