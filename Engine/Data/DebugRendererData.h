#pragma once

class RendererCore;
class ShaderManager;

struct DebugRendererData
{
public:
	RendererCore* Renderer;
	ShaderManager* Shaders;
};