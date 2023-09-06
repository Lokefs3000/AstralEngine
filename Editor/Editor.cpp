#pragma once

#include <iostream>

#include "Library.h"

#include "EdEngine.h"

int main(int argc, char* argv[])
{
#ifdef _WIN32
	std::unique_ptr<Library> sdl3Library = std::make_unique<Library>("core\\SDL3.dll");
	std::unique_ptr<Library> engineLibrary = std::make_unique<Library>("core\\Engine.dll");
#endif

	std::vector<std::string> args;
	for (size_t i = 0; i < argc; i++)
		args.push_back(argv[i]);

	EdEngine engine{};

	engine.Initialize(args);
	engine.Run();
	engine.Shutdown();

	return 0;
}