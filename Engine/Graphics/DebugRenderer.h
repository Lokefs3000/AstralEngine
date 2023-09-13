#pragma once

#include "VarData.h"

#include <memory>

#include "Basics/Initializable.h"

class Shader;
struct DebugRendererData;
class AssetManager;

class DebugRenderer : public IInitializable {
private:
	std::shared_ptr<Shader> DebugScreenShader;

	AssetManager* mR_AssetManager;

	void CreateShaders(DebugRendererData& data);
public:
	void EXPORT Initialize(InitializableBasic* data) override;
	void EXPORT Shutdown() override;

	void EXPORT Render();
};