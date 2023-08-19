#pragma once

#include "Variables.h"

class Engine;

class EXPORT EngineLayer {
private:
	Engine* m_Engine;
public:
	EngineLayer(Engine* engine);

	Engine* GetEngine();

	virtual void PreInitialize() = 0;
	virtual void OnInitialize() = 0;
	virtual void OnFrame() = 0;
	virtual void OnLateFrame() = 0;
	virtual void OnExit() = 0;
};