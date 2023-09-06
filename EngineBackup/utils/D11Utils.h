#pragma once

#include "Variables.h"

#ifdef EXPOSE_D11_UTILS
#include <Windows.h>
#include <cstdint>
#include <string>
#include <iostream>
#include <comdef.h>
#endif

class EXPORT D11Utils {
public:
#ifdef EXPOSE_D11_UTILS
	/*
		DONT CALL!
		Use "D11PerformCheck" macro instead!
	*/
	template<typename... Args>
	static void PerformCheck(HRESULT result, const char* __function, uint32_t __line, bool fatal, Args... args);
#endif
};

#ifdef EXPOSE_D11_UTILS
/*
* Check and report HRESULT errors.
* 
* \result The HRESULT
* \fatal Is the potential error fatal and thus resulting in a messagebox?
* \... Additional message(s) to pass
*/
#define D11PerformCheck(result, fatal, ...) D11Utils::PerformCheck(result, __FUNCTION__, __LINE__, fatal, __VA_ARGS__);

template<typename... Args>
inline void D11Utils::PerformCheck(HRESULT result, const char* __function, uint32_t __line, bool fatal, Args... args)
{
	if (SUCCEEDED(result))
		return;

	std::string outputMsg = "[" + std::string(__function) + ":" + std::to_string(__line) + "]: ";

	([&]
		{
			outputMsg += args;
		}(), ...);

	_com_error err(result);
	LPCTSTR errMsg = err.ErrorMessage();

	std::wstring wStr = errMsg;
	std::string errstr = std::string(wStr.begin(), wStr.end());

	outputMsg += "\nError message: " + errstr;

	if (fatal) {
		MessageBoxA(NULL, outputMsg.c_str(), "A fatal error occured!", MB_OK | MB_ICONERROR);
		exit(4); //TODO: Find a way to do this properly!
	}
	else {
		std::cout << outputMsg << std::endl; //TODO: Replace with custom logging function.
	}
}
#endif