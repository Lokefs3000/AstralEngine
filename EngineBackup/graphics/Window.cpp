#include "pch.h"
#include "Window.h"

Window::Window(uint32_t Width, uint32_t Height, std::string Title, bool IsResizable, bool IsGLWindow)
{
    uint32_t Flags = 0;
    if (IsGLWindow)
        Flags |= SDL_WINDOW_OPENGL;
    if (IsResizable)
        Flags |= SDL_WINDOW_RESIZABLE;

    m_Window = SDL_CreateWindow(Title.c_str(), Width, Height, Flags);
}

Window::~Window()
{
    SDL_DestroyWindow(m_Window);
}

bool Window::WasCloseRequested()
{
    return m_CloseRequested;
}

void Window::SetTitle(std::string newTitle)
{
    SDL_SetWindowTitle(m_Window, newTitle.c_str());
}

void Window::FeedEvent(SDL_Event& Event)
{
    if (Event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
        m_CloseRequested = true;
}

SDL_Window* Window::GetInternalWindow()
{
    return m_Window;
}
