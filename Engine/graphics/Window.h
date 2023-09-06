#pragma once

#include "VarData.h"

#ifdef ALLOW_INCLUDE
#include <SDL3/SDL_events.h>
#endif

struct SDL_Window;
struct WindowData;

class EXPORT Window {
private:
#ifdef ALLOW_INCLUDE
	SDL_Window* m_Window = NULL;
	bool m_IsClosed = false;
#endif
public:
	Window(WindowData& data);
	~Window();

	void SetBorderState(bool enabled = false);
	void SetResizable(bool enabled = false);

	void ShowWindow();
	void HideWindow();

	void Resize(uint32_t Width, uint32_t Height);
	void Center();

	bool IsClosed();

#ifdef ALLOW_INCLUDE
	void SendEvent(SDL_Event& Event);
	SDL_Window* GetWindowCore();
#endif
};