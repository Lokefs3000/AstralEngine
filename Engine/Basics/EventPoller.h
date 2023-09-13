#pragma once

#include <SDL3/SDL.h>

__interface IEventPoller
{
public:
	virtual void PushEvent(SDL_Event& Event);
};