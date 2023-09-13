#include "pch.h"
#include "EdEngine.h"

#include "SDL3/SDL.h"
#include "glad/gl.h"
#include <iostream>

#include <Data/GraphicsCoreData.h>
#include <Data/WindowData.h>
#include <Data/AssetManagerData.h>

#include <Graphics/Window.h>
#include <Graphics/GraphicsCore.h>
#include <Event/EventHandler.h>
#include <Graphics/RendererCore.h>
#include <Graphics/SwapChainManager.h>
#include <Assets/AssetManager.h>

#include "ImGui/imgui.h"

#include "ImGui/imgui_impl_sdl3.h"
#include "ImGui/imgui_impl_opengl3.h"

#include <chrono>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void EdEngine::InitializeLoadWindow()
{
	
}

void EdEngine::LoadProject(std::string dir)
{
	
}

EdEngine::EdEngine()
{
}

EdEngine::~EdEngine()
{
}

void EdEngine::Initialize(std::vector<std::string> Arguments)
{
	m_Window = std::make_shared<Window>();
	m_Graphics = std::make_shared<GraphicsCore>();
	m_EventPoller = std::make_shared<EventHandler>();
	m_EngineAssets = std::make_shared<AssetManager>();

	WindowData wdata{};
	wdata.Title = "Editor";
	wdata.Width = 1280;
	wdata.Height = 726;

	m_Window->Initialize(&wdata);

	AssetManagerData adata{};
	adata.IsPackaged = false;
	adata.SourceDir = u"core\\";

	m_EngineAssets->Initialize(&adata);

	GraphicsCoreData data{};
	data.ProductName = "Editor";
	data.TargetWindow = m_Window.get();
	data.AssetManager = m_EngineAssets.get();
	data.InitializeDebugRenderer = true;

	m_Graphics->Initialize(&data);

	m_EventPoller->Initialize(NULL);
	m_EventPoller->AddPoller(std::bind(&Window::PushEvent, m_Window.get(), std::placeholders::_1));
	m_EventPoller->AddPoller([this](SDL_Event& Event) {
		if (Event.type == SDL_EVENT_WINDOW_EXPOSED)
			m_Graphics->GetSwapChainManager()->OnMinimizeEnd();
	});
}

void EdEngine::Run()
{
	m_Graphics->RunRenderingThread(&m_SyncingMutex, &m_SyncingVariable, &m_LogicCompleted);

	while (!m_Window->IsClosed())
	{
		m_LogicCompleted = false;

		m_EventPoller->PollEvents();

		m_LogicCompleted = true;
		if (m_Window->IsMinimized()) {
			SDL_WaitEvent(NULL);
		}
		else {
			if (m_LogicCompleted && !m_Graphics->RenderingThreadCompleted) {
				std::unique_lock<std::mutex> lock(m_SyncingMutex);
				m_SyncingVariable.wait(lock, [this] { return m_Graphics->RenderingThreadCompleted; });
			}
			else {
				m_SyncingVariable.notify_all();
			}
		}
	}

	m_LogicCompleted = true;
	m_Graphics->RenderingThreadCompleted = true;
	m_SyncingVariable.notify_all();
}

void EdEngine::Shutdown()
{
	m_Graphics->Shutdown();
	m_Window->Shutdown();
}
