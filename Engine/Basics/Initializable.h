#pragma once

struct InitializableBasic
{

};

__interface IInitializable
{
public:
	virtual void Initialize(InitializableBasic* data);
	virtual void Shutdown();
};