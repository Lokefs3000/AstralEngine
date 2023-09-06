#pragma once

#include <memory>

class Window;

struct ContextData
{
public:
	std::shared_ptr<Window> MainWindow;
};