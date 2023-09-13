#pragma once

#include "VarData.h"

#include "Basics/Initializable.h"

class RendererCore;
class ShaderManager;
class AssetManager;

struct DebugRendererData : public InitializableBasic
{
public:
	RendererCore* Renderer;
	ShaderManager* Shaders;
	AssetManager* AssetManager;
};