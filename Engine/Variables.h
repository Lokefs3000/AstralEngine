#pragma once

#ifdef _WIN32
#define WINDOWS
#endif
#ifdef __EMSCRIPTEN__
#define WEB
#endif

#ifdef WINDOWS
#define D3D11
#endif
#if defined(WINDOWS) || defined(WEB)
#define OPENGL
#endif
#ifdef WEB
#define OPENGLES
#endif

#ifdef WINDOWS
#ifdef ENGINE_EXPORTS
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif
#else
#define EXPORT
#endif

#ifdef ENGINE_EXPORTS
#define EXPOSE_ENGINE
#define EXPOSE_WINDOW
#endif
