#include "pch.h"
#include "Engine.h"

#include "graphics/Window.h"
#include "graphics/GraphicsContext.h"
#include "graphics/Renderer.h"

#include "resources/AssetManager.h"
#include "resources/AssetManagerRaw.h"

#include "graphics/managers/TextureManager.h"
#include "graphics/d11/managers/D11TextureManager.h"

#include "scenes/SceneManager.h"

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
	m_ProjectFolder = config.substr(0, config.find_last_of("\\"));
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
		m_TextureManager = std::make_shared<D11TextureManager>();
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

	if (ProjectConfig->GetChild("ProjectConfig")->GetChild("AssetSettings")->GetValue<bool>("IsRaw"))
		m_AssetManager = std::make_shared<AssetManagerRaw>();

	m_AssetManager->Initialize(m_ProjectFolder + "\\assets\\");

	m_GraphicsContext->InitializeContext(m_MainWindow);
	m_TextureManager->Initialize(m_GraphicsContext, m_AssetManager, ProjectConfig->GetChild("ProjectConfig")->GetChild("TextureManager")->GetValue<bool>("ThreadingEnabled"));
	m_Renderer->InitializeRenderer(m_MainWindow, m_GraphicsContext);

	m_SceneManager = std::make_shared<SceneManager>();

	for (size_t i = 0; i < EngineLayers.size(); i++)
	{
		EngineLayers[i]->OnInitialize();
	}

	Watches.DeltaWatch = newStopWatch();
	Watches.RenderWatch = newStopWatch();
	Watches.AssetWatch = newStopWatch();
	Watches.TextureWatch = newStopWatch();
	Watches.LayerWatch = newStopWatch();
	Watches.LateLayerWatch = newStopWatch();
	Watches.EventWatch = newStopWatch();
}

void Engine::Run()
{
	SDL_Event Event;

	while (!m_MainWindow->WasCloseRequested())
	{
		Watches.DeltaWatch->Start();

		Watches.EventWatch->Start();
		while (SDL_PollEvent(&Event))
		{
			m_MainWindow->FeedEvent(Event);

			for (auto layer : EngineLayers)
				layer->OnEvent(Event);
		}
		Watches.EventWatch->End();

		m_Renderer->ClearScreen();

		Watches.LayerWatch->Start();
		for (auto layer : EngineLayers)
			layer->OnFrame();
		Watches.LayerWatch->End();

		Watches.RenderWatch->Start();
		Watches.RenderWatch->End();

		m_Renderer->PresentScreen();

		Watches.LateLayerWatch->Start();
		for (auto layer : EngineLayers)
			layer->OnLateFrame();
		Watches.LateLayerWatch->End();

		Watches.DeltaWatch->End();
	}
}

void Engine::Shutdown()
{
	for (size_t i = 0; i < EngineLayers.size(); i++)
	{
		EngineLayers[i]->OnExit();
		EngineLayers[i].reset();
	}
	EngineLayers.clear();

	m_TextureManager->Shutdown();

	m_Renderer->ShutdownRenderer();
	m_GraphicsContext->ShutdownContext();

	m_TextureManager.reset();
	m_AssetManager.reset();
	m_Renderer.reset();
	m_GraphicsContext.reset();
	m_MainWindow.reset();

	D11GraphicsContext::ObjectReport();
}
