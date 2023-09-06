#include "pch.h"
#include "D11Renderer.h"

#include "graphics/Window.h"
#include "graphics/d11/D11GraphicsContext.h"
#include "managers/D11TextureManager.h"
#include "resources/D11RenderTexture.h"
#include "scenes/Scene.h"

#include "scenes/components/Camera.h"
#include "scenes/components/SpriteRenderer.h"
#include "scenes/components/ui/Image.h"
#include "scenes/components/Canvas.h"
#include "scenes/GameObject.h"
#include "scenes/components/Transform.h"

void D11Renderer::GetBackBuffers()
{
	D11PerformCheck(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_BackBufferTexture.GetAddressOf()), false, "Failed to get backbuffer!");

	D3D11_RENDER_TARGET_VIEW_DESC backViewDesc{};
	backViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	backViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	
	D11PerformCheck(m_Context->GetDevice()->CreateRenderTargetView(m_BackBufferTexture.Get(), &backViewDesc, m_BackBufferView.GetAddressOf()), true, "SwapChain resizing failed!");

	m_Context->GetImmediate()->OMSetRenderTargets(1, m_BackBufferView.GetAddressOf(), NULL);
	m_Context->GetDeferred()->OMSetRenderTargets(1, m_BackBufferView.GetAddressOf(), NULL);
}

void D11Renderer::RefreshBuffers(uint32_t w, uint32_t h)
{
	m_BackBufferTexture.Reset();
	m_BackBufferView.Reset();

	D11PerformCheck(m_SwapChain->ResizeBuffers(2, w, h, DXGI_FORMAT_R8G8B8A8_UNORM, 0), false, "Failed to resize buffers!");
	m_PrimaryRender->Resize(glm::ivec2(w, h));

	GetBackBuffers();
}

void D11Renderer::ParseEntity(RendererRenderSceneData& data, std::shared_ptr<Scene> scene, std::shared_ptr<GameObject> objects, glm::mat4 pModel)
{
	std::shared_ptr<EntityComponents> components = objects->Components;

	Transform* transform = components->GetComponent<Transform>();

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, transform->Position);
	model = glm::rotate(model, glm::radians(transform->Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(transform->Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(transform->Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, transform->Scale);
	model = pModel * model;

	Canvas* canvas = components->GetComponent<Canvas>();


	Image* canvas = components->GetComponent<Image>();

	for (auto entity : objects->Children)
	{
		if (entity->Parent.get() == NULL) {
			ParseEntity(data, scene, entity, glm::mat4(model));
		}
	}
}

RendererRenderSceneData D11Renderer::ParseScene(std::shared_ptr<Scene> scene)
{
	RendererRenderSceneData sceneData{};

	for (auto entity : scene->GetEntities())
	{
		if (entity->Parent.get() == NULL) {
			ParseEntity(sceneData, scene, entity, glm::mat4(1.0f));
		}
	}

	return sceneData;
}

void D11Renderer::RenderScene(std::shared_ptr<Scene> scene)
{
	RendererRenderSceneData data = ParseScene(scene);
}

void D11Renderer::InitializeRenderer(std::shared_ptr<ConfigObject> config, std::shared_ptr<Window> window, std::shared_ptr<IGraphicsContext> graphicsContext, std::shared_ptr<ITextureManager> textures)
{
	D11GraphicsContext* ctx = (D11GraphicsContext*)graphicsContext.get();
	m_Context = ctx;

	int wW, wH;
	SDL_GetWindowSizeInPixels(window->GetInternalWindow(), &wW, &wH);

	SDL_SysWMinfo windowInfo;
	SDL_GetWindowWMInfo(window->GetInternalWindow(), &windowInfo, SDL_SYSWM_CURRENT_VERSION);

	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferDesc.Width = wW;
	swapChainDesc.BufferDesc.Height = wH;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferCount = 2; //Double buffer no question!
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = windowInfo.info.win.window;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	D11PerformCheck(ctx->GetFactory()->CreateSwapChain(ctx->GetDevice(), &swapChainDesc, m_SwapChain.GetAddressOf()), true, "Failed to create SwapChain!");

	GetBackBuffers();

	D11TextureManager* tm = (D11TextureManager*)textures.get();

	RenderTextureOptions renderMainOptions{};
	renderMainOptions.Channels = 4;
	renderMainOptions.Context = m_Context;
	renderMainOptions.Filtering = 0;
	renderMainOptions.Repeating = 1;
	renderMainOptions.Width = 1;
	renderMainOptions.Height = 1;

	D3D11_BUFFER_DESC uiDesc{};
	uiDesc.ByteWidth = sizeof(glm::vec4);
	uiDesc.Usage = D3D11_USAGE_DYNAMIC;
	uiDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D11PerformCheck(ctx->GetDevice()->CreateBuffer(&uiDesc, NULL, m_UIBuffer.GetAddressOf()), true, "Failed to create UI buffer!");

	m_PrimaryRenderRef = tm->GetRenderTexture(renderMainOptions, "PRIMARY_RENDER_ENGINE_TARGET");
	m_PrimaryRender = (D11RenderTexture*)tm->GetRenderTextureFromRef(m_PrimaryRenderRef).get();
	m_PrimaryRender->Resize(glm::ivec2(wW, wH));

	m_ShowRender = config->GetValue<bool>("RenderToScreen");
}

void D11Renderer::ShutdownRenderer()
{
	m_Context = NULL;
}

void D11Renderer::ClearScreen()
{
	float clear[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
	m_Context->GetDeferred()->ClearRenderTargetView(m_BackBufferView.Get(), clear);
}

void D11Renderer::RenderMain(std::vector<std::shared_ptr<Scene>> scenes)
{
	auto target = m_PrimaryRender->GetRenderTarget();
	m_Context->GetDeferred()->OMSetRenderTargets(1, &target, NULL);
	ClearScreen();

	for (auto scene : scenes)
		RenderScene(scene);

	m_Context->GetDeferred()->OMSetRenderTargets(1, m_BackBufferView.GetAddressOf(), NULL);
	ClearScreen();
}

void D11Renderer::PresentScreen()
{
	ID3D11CommandList* cmdList = NULL;
	D11PerformCheck(m_Context->GetDeferred()->FinishCommandList(true, &cmdList), false, "Failed to finish command list!");
	if (!cmdList == NULL) {
		m_Context->GetImmediate()->ExecuteCommandList(cmdList, false);
		cmdList->Release();
	}

	D11PerformCheck(m_SwapChain->Present(1, 0), false, "Failed to present SwapChain");
}

void D11Renderer::ToggleShowRender()
{
	m_ShowRender = !m_ShowRender;
}

GraphicsAPI D11Renderer::GetAPI()
{
	return GraphicsAPI::Direct3D11;
}
