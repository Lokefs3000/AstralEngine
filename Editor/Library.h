#pragma once

#include <string>
#include <windows.h> 
#include <iostream>

typedef int(__cdecl* MYPROC)(LPCWSTR);

class Library {
private:
	HMODULE m_Module;
public:
	Library(std::string dll);
	~Library();
};