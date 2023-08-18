#include "pch.h"
#include "Engine.h"

#include "graphics/Window.h"
#include "graphics/GraphicsContext.h"
#include "graphics/Renderer.h"

#ifdef WINDOWS
#include "graphics/d11/D11GraphicsContext.h"
#include "graphics/d11/D11Renderer.h"
#endif

void Engine::Initialize()
{
	m_MainWindow = std::make_shared<Window>();

	GraphicsAPI pApi = ApiUtils::GetAPI();

	switch (pApi)
	{
#ifdef WINDOWS
	case GraphicsAPI::Direct3D11:
		m_GraphicsContext = std::make_shared<D11GraphicsContext>();
		m_Renderer = std::make_shared<D11Renderer>();
		break;
#endif
	case GraphicsAPI::OpenGL:
		break;
	case GraphicsAPI::OpenGLES:
		break;
	default:
		break;
	}

	m_GraphicsContext->InitializeContext(m_MainWindow);
	m_Renderer->InitializeRenderer(m_MainWindow, m_GraphicsContext);

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
