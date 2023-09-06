#include "pch.h"
#include "GLGraphicsContext.h"

#include <glad/gl.h>
#include <SDL3/SDL.h>

#include "Graphics/Window.h"

#include "Data/ContextData.h"
#include "Data/Enum/GraphicsAPI.h"

#include "Debug/Logger.h"

static void GLAPIENTRY MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	if (type == GL_DEBUG_TYPE_ERROR) {
		Error("OpenGL error occured! Type: ", type, ", Id: ", id, ", Severity: ", severity, ", Message: ", message);
	}
	else {
		//Warning("OpenGL error occured! Type: ", type, ", Id: ", id, ", Severity: ", severity, ", Message: ", message);
	}
}

void GLGraphicsContext::Initialize(ContextData& data)
{
	m_Window = data.MainWindow->GetWindowCore();

	m_GL = SDL_GL_CreateContext(m_Window);
	SDL_GL_MakeCurrent(m_Window, m_GL);

	gladLoadGL(SDL_GL_GetProcAddress);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, NULL);
}

void GLGraphicsContext::Shutdown()
{
	gladLoaderUnloadGL();
	SDL_GL_DeleteContext(m_GL);
}

void GLGraphicsContext::ClearFrame()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void GLGraphicsContext::PresentFrame()
{
	SDL_GL_SwapWindow(m_Window);
}

GraphicsAPI GLGraphicsContext::GetGraphicsId()
{
	return GraphicsAPI::OpenGL;
}

SDL_GLContext GLGraphicsContext::GetContext()
{
	return m_GL;
}
