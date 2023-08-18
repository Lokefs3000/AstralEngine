#pragma once

#include "Variables.h"

enum class GraphicsAPI
{
	Direct3D11,
	OpenGL,
	OpenGLES
};

class ApiUtils {
public:
	static GraphicsAPI GetAPI();
};