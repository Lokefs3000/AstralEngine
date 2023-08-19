#include <core/Engine.h>
#include <core/EngineLayer.h>

#define WIN32 //IDK
#ifdef WIN32
#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include "Library.h"
#endif

#include <fstream>
#include <sstream>

class EditorLayer : public EngineLayer {
public:
    using EngineLayer::EngineLayer;

    void PreInitialize() override;
    void OnInitialize() override;
    void OnFrame() override;
    void OnLateFrame() override;
    void OnExit() override;
};

int main()
{
#ifdef WIN32
    std::unique_ptr<Library> sdl3Lib = std::make_unique<Library>("core\\SDL3.dll");
    std::unique_ptr<Library> engineLib = std::make_unique<Library>("core\\Engine.dll");
#endif

   Engine engine = Engine();

   engine.AddEngineLayer<EditorLayer>();

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
    
}

void EditorLayer::OnFrame()
{
}

void EditorLayer::OnLateFrame()
{
}

void EditorLayer::OnExit()
{
}
