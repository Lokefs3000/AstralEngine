#pragma once

#include "VarData.h"
#include "Basics/GraphicsContext.h"

#ifdef ALLOW_INCLUDE
typedef void* SDL_GLContext;
struct SDL_Window;
#endif

class GLGraphicsContext : public IGraphicsContext {
private:
#ifdef ALLOW_INCLUDE
	SDL_GLContext m_GL;
	SDL_Window* m_Window;
#endif
public:
	void EXPORT Initialize(ContextData& data) override;
	void EXPORT Shutdown() override;

	void EXPORT ClearFrame();
	void EXPORT PresentFrame();

	GraphicsAPI EXPORT GetGraphicsId() override;

#ifdef ALLOW_INCLUDE
	SDL_GLContext EXPORT GetContext();
#endif
};