#pragma once

#ifdef ASHADER_EXPORTS
#define ASEXPORT __declspec(dllexport)
#else
#define ASEXPORT __declspec(dllimport)
#endif