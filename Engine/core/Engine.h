#pragma once

#include "../Variables.h"

#include "EngineLayer.h"

#include <vector>
#include <memory>
#include <type_traits>

class Engine {
private:

public:
	std::vector<std::shared_ptr<IEngineLayer>> EngineLayers;

	template<typename T>
	std::shared_ptr<T> AddEngineLayer();

	void Initialize();
	void Run();
	void Shutdown();
};

#ifdef EXPOSE_ENGINE
template<typename T>
inline std::shared_ptr<T> Engine::AddEngineLayer()
{
	static_assert(std::is_same_v<T, IEngineLayer>, "T is not a child of IEngineLayer");

	std::shared_ptr<IEngineLayer> layer = std::make_shared<T>();
	EngineLayers.push_back(layer);

	return layer;
}
#endif