#include "pch.h"
#include "ApiUtils.h"

GraphicsAPI ApiUtils::GetAPI()
{
#ifdef WEB
	return GraphicsAPI::OpenGLES;
#endif
#ifdef WINDOWS
	return GraphicsAPI::Direct3D11;
#endif
	return GraphicsAPI::OpenGL;
}
