#include "pch.h"
#include "Engine.h"

void Engine::Initialize()
{
	for (size_t i = 0; i < EngineLayers.size(); i++)
	{
		EngineLayers[i]->OnInitialize();
	}
}

void Engine::Run()
{
}

void Engine::Shutdown()
{
	for (size_t i = 0; i < EngineLayers.size(); i++)
	{
		EngineLayers[i]->OnExit();
	}
}
