#pragma once

#include "VarData.h"

#ifdef ALLOW_INCLUDE
#include <memory>
#endif

class Window;
__interface IGraphicsContext;
__interface ITextureManager;

class EXPORT Engine {
private:
#ifdef ALLOW_INCLUDE
	
#endif
	std::unique_ptr<Window> m_MainWindow;
	std::unique_ptr<IGraphicsContext> m_Context;

	void InitializeLoadWindow();
public:
	Engine();
	~Engine();

	void Initialize();
};