#pragma once

#include "Variables.h"

#ifdef EXPOSE_WINDOW
#include <SDL3/SDL.h>
#include <string>
#endif

class EXPORT Window {
private:
#ifdef EXPOSE_WINDOW
	SDL_Window* m_Window = NULL;

	bool m_CloseRequested = false;
#endif
public:
	Window(uint32_t Width, uint32_t Height, std::string Title, bool IsResizable, bool IsGLWindow);
	~Window();

	bool WasCloseRequested();

	void SetTitle(std::string newTitle);

#ifdef EXPOSE_WINDOW
	void FeedEvent(SDL_Event& Event);
	SDL_Window* GetInternalWindow();
#endif

	DEFINED_AS
};
