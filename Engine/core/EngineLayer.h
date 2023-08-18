#pragma once

__interface IEngineLayer {
public:
	virtual void OnInitialize();
	virtual void OnFrame();
	virtual void OnLateFrame();
	virtual void OnExit();
};