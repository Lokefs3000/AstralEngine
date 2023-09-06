#pragma once

struct ContextData;
enum class GraphicsAPI;

__interface IGraphicsContext {
public:
	virtual void Initialize(ContextData& data);
	virtual void Shutdown();

	virtual void ClearFrame();
	virtual void PresentFrame();

	virtual GraphicsAPI GetGraphicsId();
};