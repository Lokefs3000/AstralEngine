#pragma once

#include "Variables.h"

#ifdef LAYER_EVENTS
#include <SDL3/SDL.h>
#endif

class Engine;

class EXPORT EngineLayer {
private:
	Engine* m_Engine;
public:
	EngineLayer(Engine* engine);

	Engine* GetEngine();

	virtual void PreInitialize() = 0;
	virtual void OnInitialize() = 0;
	virtual void OnFrame() = 0;
	virtual void OnLateFrame() = 0;
	virtual void OnExit() = 0;

#ifdef LAYER_EVENTS
	virtual void OnEvent(SDL_Event& Event);
#endif
};