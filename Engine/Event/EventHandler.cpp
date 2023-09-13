#include "pch.h"
#include "EventHandler.h"

void EventHandler::Initialize(InitializableBasic* data)
{
}

void EventHandler::Shutdown()
{
}

void EventHandler::PollEvents()
{
	SDL_Event Event;
	while (SDL_PollEvent(&Event))
	{
		for (size_t i = 0; i < m_Events.size(); i++)
			m_Events[i](Event);
	}
}

void EventHandler::AddPoller(std::function<void(SDL_Event&)> func)
{
	m_Events.push_back(func);
}
