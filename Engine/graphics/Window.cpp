#include "pch.h"
#include "Window.h"

#include "Data/WindowData.h"

void Window::Initialize(InitializableBasic* data)
{
	WindowData& mdata = *(WindowData*)data;

	m_Window = SDL_CreateWindow(mdata.Title.c_str(), mdata.Width, mdata.Height, SDL_WINDOW_VULKAN);
}

void Window::Shutdown()
{
	SDL_DestroyWindow(m_Window);
}

void Window::PushEvent(SDL_Event& Event)
{
	if (Event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
		m_IsClosed = true;
	} else if (Event.type == SDL_EVENT_WINDOW_MINIMIZED) {
		m_IsMinimized = true;
	}
	else if (Event.type == SDL_EVENT_WINDOW_SHOWN) {
		m_IsMinimized = false;
	}
}
