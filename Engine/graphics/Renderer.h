#pragma once

#include "Variables.h"

#include "GraphicsContext.h"
#include <memory>

class Window;

__interface IRenderer
{
public:
	virtual void InitializeRenderer(std::shared_ptr<Window> window, std::shared_ptr<IGraphicsContext> graphicsContext);
	virtual void ShutdownRenderer();

	virtual void ClearScreen();
	virtual void PresentScreen();
};