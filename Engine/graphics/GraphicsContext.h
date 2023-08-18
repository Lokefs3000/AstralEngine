#pragma once

#include "Variables.h"

#include <memory>

class Window;

__interface IGraphicsContext
{
public:
	virtual void InitializeContext(std::shared_ptr<Window> window);
	virtual void ShutdownContext();
};