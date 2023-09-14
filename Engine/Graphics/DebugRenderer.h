#pragma once

#include "VarData.h"

#include <memory>

#include "Basics/Initializable.h"

class Shader;
struct DebugRendererData;
class AssetManager;
class BufferManager;
class Buffer;
class RendererCore;

class DebugRenderer : public IInitializable {
private:
	std::shared_ptr<Shader> m_DebugScreenShader;
	std::shared_ptr<Buffer> m_DebugBuffer;

	AssetManager* mR_AssetManager;
	BufferManager* mR_BufferManager;
	RendererCore* mR_Renderer;

	void CreateShaders(DebugRendererData& data);
	void CreateBuffers(DebugRendererData& data);
public:
	void EXPORT Initialize(InitializableBasic* data) override;
	void EXPORT Shutdown() override;

	void EXPORT Render();
};