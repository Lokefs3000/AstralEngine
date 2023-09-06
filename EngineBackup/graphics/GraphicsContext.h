#pragma once

#include "Variables.h"

#include <memory>

enum class GraphicsAPI;
class Window;

__interface IGraphicsContext
{
public:
	virtual void EXPORT InitializeContext(std::shared_ptr<Window> window);
	virtual void EXPORT ShutdownContext();

	virtual EXPORT GraphicsAPI GetAPI();
};