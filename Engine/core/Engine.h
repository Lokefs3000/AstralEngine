#pragma once

#include "../Variables.h"

#include "EngineLayer.h"

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

	std::vector<std::shared_ptr<IEngineLayer>> EngineLayers = std::vector<std::shared_ptr<IEngineLayer>>();

	template<typename T>
	std::shared_ptr<T> AddEngineLayer();

	void Initialize();
	void Run();
	void Shutdown();

	DEFINED_AS
};

template<typename T>
inline std::shared_ptr<T> Engine::AddEngineLayer()
{
	static_assert(std::is_base_of_v<IEngineLayer, T>, "T is not a child of IEngineLayer");

	std::shared_ptr<T> layer = std::make_shared<T>();
	EngineLayers.push_back(layer);

	return layer;
}