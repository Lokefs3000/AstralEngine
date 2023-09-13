#pragma once

#include "VarData.h"

#include <vector>
#include <functional>

#include <SDL3/SDL.h>

#include "Basics/Initializable.h"

class EventHandler : public IInitializable {
private:
	std::vector<std::function<void(SDL_Event&)>> m_Events;
public:
	void EXPORT Initialize(InitializableBasic* data) override;
	void EXPORT Shutdown() override;

	void EXPORT PollEvents();

	void EXPORT AddPoller(std::function<void(SDL_Event&)> func);
};