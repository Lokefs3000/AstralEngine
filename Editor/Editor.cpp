#include <core/Engine.h>

#define WIN32 //IDK
#ifdef WIN32
#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include "Library.h"
#endif

int main()
{
#ifdef WIN32
    std::unique_ptr<Library> sdl3Lib = std::make_unique<Library>("core\\SDL3.dll");
    std::unique_ptr<Library> engineLib = std::make_unique<Library>("core\\Engine.dll");
#endif

    std::unique_ptr<Engine> engine = std::make_unique<Engine>();

    engine->Initialize();
    engine->Run();
    engine->Shutdown();

    engine.reset();
    engineLib.reset();
    sdl3Lib.reset();

    return 0;
}