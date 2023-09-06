#pragma once

#include <string>

struct WindowData
{
	uint32_t Width;
	uint32_t Height;

	std::string Title;

	bool IsBorderless;
	bool IsResizable;
	bool IsOpenGL;
	bool IsHidden;
};