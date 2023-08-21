#pragma once

#include "Variables.h"

#include <memory>

enum class GraphicsAPI;
class Window;

__interface IGraphicsContext
{
public:
	virtual void InitializeContext(std::shared_ptr<Window> window);
	virtual void ShutdownContext();

	virtual GraphicsAPI GetAPI();
};