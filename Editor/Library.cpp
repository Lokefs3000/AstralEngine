#include "Library.h"

#ifdef _WIN32
Library::Library(std::string dll)
{
	m_Module = LoadLibraryExA(dll.c_str(), NULL, 0);
	if (m_Module == NULL) {
		std::cout << "Failed to execute LoadPackagedLibrary on file: " << dll << ", error: " << GetLastError() << std::endl;
		return;
	}
}

Library::~Library()
{
	FreeLibrary(m_Module);
}
#endif
