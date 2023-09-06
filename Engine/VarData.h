#pragma once

#ifdef ENGINE_EXPORTS
#define ALLOW_INCLUDE
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif
