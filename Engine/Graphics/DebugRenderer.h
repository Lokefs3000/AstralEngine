#pragma once

#include <memory>

#include "Basics/Initializable.h"

class Shader;
struct DebugRendererData;

class DebugRenderer : public IInitializable {
private:
	std::shared_ptr<Shader> DebugScreenShader;

	void CreateShaders(DebugRendererData& data);
public:
	void Initialize(InitializableBasic* data) override;
	void Shutdown() override;
};