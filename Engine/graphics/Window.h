#pragma once

#include "VarData.h"

#include <SDL3/SDL.h>

#include "Basics/Initializable.h"
#include "Basics/EventPoller.h"

struct WindowData;

class Window : public IInitializable,
			   public IEventPoller {
private:
	SDL_Window* m_Window;

	bool m_IsClosed = false;
	bool m_IsMinimized = false;
public:
	void EXPORT Initialize(InitializableBasic* data) override;
	void EXPORT Shutdown() override;

	void EXPORT PushEvent(SDL_Event& Event) override;

	bool IsMinimized() { return m_IsMinimized; }
	bool IsClosed() { return m_IsClosed; }
	SDL_Window* GetSDLWindow() { return m_Window; }
};