#pragma once

#include "Variables.h"

#include "utils/ApiUtils.h"

#include <memory>
#include <pair>

__interface IGraphicsContext;
__interface ITextureManager;
class Window;
struct ConfigObject;
class Scene;

struct Component;
struct Camera;
struct Canvas;

struct RendererRenderSceneData
{
public:
	Camera* Camera;

	std::vector<std::pair<glm::mat4, Component*>> OpaqueMeshes;
	std::vector<std::pair<glm::mat4, Component*>> TransparentMeshes;

	std::vector<std::pair<Canvas*, std::vector<std::pair<glm::mat4, Component*>>>> UIComponents;
};

__interface IRenderer
{
public:
	virtual void InitializeRenderer(std::shared_ptr<ConfigObject> config, std::shared_ptr<Window> window, std::shared_ptr<IGraphicsContext> graphicsContext, std::shared_ptr<ITextureManager> textures);
	virtual void ShutdownRenderer();

	virtual void ClearScreen();
	virtual void RenderMain(std::vector<std::shared_ptr<Scene>> scenes);
	virtual void PresentScreen();

	virtual void ToggleShowRender();

	virtual GraphicsAPI GetAPI();
};