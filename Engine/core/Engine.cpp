#include "pch.h"
#include "Engine.h"

#include "graphics/Window.h"
#include "graphics/GraphicsContext.h"
#include "graphics/Renderer.h"

#ifdef WINDOWS
#include "graphics/d11/D11GraphicsContext.h"
#include "graphics/d11/D11Renderer.h"
#endif

Engine::Engine()
{
}

Engine::~Engine()
{
}

void Engine::LoadProjectConfig(std::string config)
{
	ProjectConfig = Configurations::LoadConfig(config);
}

void Engine::Initialize()
{
	for (size_t i = 0; i < EngineLayers.size(); i++)
	{
		EngineLayers[i]->PreInitialize();
	}

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

	std::shared_ptr<ConfigObject> windowOptions = ProjectConfig->GetChild("ProjectConfig")->GetChild("WindowSettings");
	m_MainWindow = std::make_shared<Window>(windowOptions->GetValue<int>("Width"), windowOptions->GetValue<int>("Height"), StringUtils::KeywordFormat(windowOptions->GetValue<std::string>("Title")), true, pApi != GraphicsAPI::Direct3D11 ? true : false);

	m_GraphicsContext->InitializeContext(m_MainWindow);
	m_Renderer->InitializeRenderer(m_MainWindow, m_GraphicsContext);

	for (size_t i = 0; i < EngineLayers.size(); i++)
	{
		EngineLayers[i]->OnInitialize();
	}
}

void Engine::Run()
{
	SDL_Event Event;

	while (!m_MainWindow->WasCloseRequested())
	{
		while (SDL_PollEvent(&Event))
		{
			m_MainWindow->FeedEvent(Event);

			for (auto layer : EngineLayers)
				layer->OnEvent(Event);
		}

		m_Renderer->ClearScreen();

		for (auto layer : EngineLayers)
			layer->OnFrame();

		m_Renderer->PresentScreen();

		for (auto layer : EngineLayers)
			layer->OnLateFrame();
	}
}

void Engine::Shutdown()
{
	for (size_t i = 0; i < EngineLayers.size(); i++)
	{
		EngineLayers[i]->OnExit();
	}

	EngineLayers.clear();

	m_Renderer->ShutdownRenderer();
	m_GraphicsContext->ShutdownContext();

	m_Renderer.reset();
	m_GraphicsContext.reset();
	m_MainWindow.reset();
}
