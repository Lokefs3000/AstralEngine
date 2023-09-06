#include "pch.h"
#include "Window.h"

#include <SDL3/SDL.h>
#include "Data/WindowData.h"

Window::Window(WindowData& data)
{
	uint32_t flags = 0;
	flags |= data.IsBorderless ? SDL_WINDOW_BORDERLESS : 0;
	flags |= data.IsResizable ? SDL_WINDOW_RESIZABLE : 0;
	flags |= data.IsOpenGL ? SDL_WINDOW_OPENGL : 0;
	flags |= data.IsHidden ? SDL_WINDOW_HIDDEN : 0;

	m_Window = SDL_CreateWindow(data.Title.c_str(), data.Width, data.Height, flags);
}

Window::~Window()
{
	SDL_DestroyWindow(m_Window);
}

void Window::SetBorderState(bool enabled)
{
	SDL_SetWindowBordered(m_Window, (SDL_bool)enabled);
}

void Window::SetResizable(bool enabled)
{
	SDL_SetWindowResizable(m_Window, (SDL_bool)enabled);
}

void Window::ShowWindow()
{
	SDL_ShowWindow(m_Window);
}

void Window::HideWindow()
{
	SDL_HideWindow(m_Window);
}

void Window::Resize(uint32_t Width, uint32_t Height)
{
	SDL_SetWindowSize(m_Window, Width, Height);
}

void Window::Center()
{
	SDL_SetWindowPosition(m_Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

bool Window::IsClosed()
{
	return m_IsClosed;
}

void Window::SendEvent(SDL_Event& Event)
{
	if (Event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
		m_IsClosed = true;
	}
}

SDL_Window* Window::GetWindowCore()
{
	return m_Window;
}
