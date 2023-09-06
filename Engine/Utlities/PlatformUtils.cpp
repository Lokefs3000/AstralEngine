#include "pch.h"
#include "PlatformUtils.h"

#include "Debug/Logger.h"

#ifdef _WIN32
#include <windows.h>

typedef const char* (__stdcall* pwine_get_version)(void);
#endif

bool PlatformUtils::IsWine()
{
#ifdef _WIN32
    HMODULE hntdll = GetModuleHandle(TEXT("ntdll.dll"));
    if (!hntdll) {
        Debug("Not running on NT, might be another platform.");
        return false;
    }
    pwine_get_version get_version = NULL;
    get_version = (pwine_get_version)GetProcAddress(hntdll, "wine_get_version");
    FreeModule(hntdll);
    if (get_version) {
        const char* version = get_version();
        Debug("Wine detected! Using version: ", version, "..");
        return true;
    }
    else {
        Debug("Wine not detected! Likely on windows..");
        return false;
    }
#else
    return false;
#endif
}

bool PlatformUtils::IsWindows()
{
    return !IsWine();
}
