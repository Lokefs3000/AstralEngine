#define EXPOSE_WINDOW
#define EXPOSE_CONTEXT
#define EXPOSE_RENDERER
#define LAYER_EVENTS
#define EXPOSE_TEXTURE2D

#include <core/Engine.h>
#include <core/EngineLayer.h>

#include <graphics/d11/D11GraphicsContext.h>
#include <graphics/d11/D11Renderer.h>
#include <graphics/Window.h>
#include <graphics/d11/managers/D11TextureManager.h>
#include <graphics/d11/resources/D11Texture2D.h>

#include <resources/AssetManager.h>
#include <resources/AssetManagerRaw.h>

#define WIN32 //IDK
#ifdef WIN32
#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include "Library.h"
#endif

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_sdl3.h"

#include "ContentBrowser.h"
#include "PerformanceViewer.h"

#include <fstream>
#include <sstream>
#include <filesystem>

class EditorLayer : public EngineLayer {
private:
    std::string m_ProjectFolder;
    std::shared_ptr<ConfigObject> m_LoadedConfig;

    std::shared_ptr<ConfigObject> m_InterfaceConfig;

    std::shared_ptr<ContentBrowser> m_ContentBrowser;
    std::shared_ptr<PerformanceViewer> m_PerformanceViewer;

    std::shared_ptr<IAssetManager> m_AssetManager;
    std::shared_ptr<ITextureManager> m_TextureManager;
public:
    using EngineLayer::EngineLayer;

    void PreInitialize() override;
    void OnInitialize() override;
    void OnFrame() override;
    void OnLateFrame() override;
    void OnExit() override;

    void OnEvent(SDL_Event& Event) override;

    void SetProject(std::string proj);
};

int main(int argc, char* argv[])
{
#ifdef WIN32
    std::unique_ptr<Library> sdl3Lib = std::make_unique<Library>("core\\SDL3.dll");
    std::unique_ptr<Library> engineLib = std::make_unique<Library>("core\\Engine.dll");
#endif

   Engine engine = Engine();

   auto layer = engine.AddEngineLayer<EditorLayer>();
   
   for (size_t i = 0; i < argc; i++)
   {
       std::string arg = argv[i];
       if (arg == "-open") {
           i++;
           layer->SetProject(argv[i]);
       }
   }

   engine.Initialize();
   engine.Run();
   engine.Shutdown();

    return 0;
}

void EditorLayer::PreInitialize()
{
    std::ifstream cfile("editor\\config\\editor.astral");
    std::stringstream src;
    src << cfile.rdbuf();
    cfile.close();

    GetEngine()->LoadProjectConfig(src.str());
}

void EditorLayer::OnInitialize()
{
    auto window = GetEngine()->GetWindow();
    auto context = (D11GraphicsContext*)GetEngine()->GetContext().get();
    auto renderer = (D11Renderer*)GetEngine()->GetRenderer().get();

    ImGui::CreateContext();
    ImGui_ImplSDL3_InitForD3D(window->GetInternalWindow());
    ImGui_ImplDX11_Init(context->GetDevice(), context->GetDeferred());

    if (m_LoadedConfig.get() != NULL)
        window->SetTitle(StringUtils::KeywordFormat("AstralEditor | Loaded project: " + m_LoadedConfig->GetChild("ProjectConfig")->GetValue<std::string>("DisplayName") + " | Engine: v{%eVer}"));

    std::ifstream ifile("editor\\config\\interface.astral");
    std::stringstream isrc;
    isrc << ifile.rdbuf();
    ifile.close();

    m_InterfaceConfig = Configurations::LoadConfig(isrc.str());

    m_ContentBrowser = std::make_shared<ContentBrowser>();
    m_PerformanceViewer = std::make_shared<PerformanceViewer>();

    m_AssetManager = std::make_shared<AssetManagerRaw>();
    m_AssetManager->Initialize(m_ProjectFolder);

    m_TextureManager = std::make_shared<D11TextureManager>();
    m_TextureManager->Initialize(GetEngine()->GetContext(), m_AssetManager, false);
}

void EditorLayer::OnFrame()
{
    ImGui_ImplSDL3_NewFrame();
    ImGui_ImplDX11_NewFrame();
    ImGui::NewFrame();

    m_ContentBrowser->Render(m_TextureManager, m_ProjectFolder + "\\assets\\");
    m_PerformanceViewer->Render(GetEngine());

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void EditorLayer::OnLateFrame()
{
    
}

void EditorLayer::OnExit()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

void EditorLayer::OnEvent(SDL_Event& Event)
{
    ImGui_ImplSDL3_ProcessEvent(&Event);
}

void EditorLayer::SetProject(std::string proj)
{
    if (!std::filesystem::is_directory(proj))
        return;
    if (!std::filesystem::exists(proj + "\\project.astral"));

    m_ProjectFolder = proj;

    std::ifstream cfile(proj + "\\project.astral");
    std::stringstream src;
    src << cfile.rdbuf();
    cfile.close();

    m_LoadedConfig = Configurations::LoadConfig(src.str());
}
