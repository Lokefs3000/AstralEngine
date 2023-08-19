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
#define EXPOSE_CONTEXT
#define EXPOSE_RENDERER
#define EXPOSE_D11_UTILS
#endif

#define DEFINED_AS template<typename T> T* As() { return (T*)this; }

#define VERSION_MAJOR 0 //Big updates (things like a whole new animation system or big featues)
#define VERSION_MINOR 1 //Small updates (small additions like an extra options for that)
#define VERSION_PATCH 0 //Patch updates (fixing bugs and broken features)

#define VERSION_WHOLE (VERSION_PATCH << 20) | (VERSION_MINOR << 10) | VERSION_MAJOR

#define VERSION_GET_MAJOR(version) version >> 20
#define VERSION_GET_MINOR(version) (combined >> 10) & 0x3ff
#define VERSION_GET_PATCH(version) combined & 0x3ff