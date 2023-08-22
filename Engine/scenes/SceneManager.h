#pragma once

#include "Variables.h"

#include <vector>
#include <memory>
#include <string>

class Scene;

class EXPORT SceneManager {
private:
	std::vector<std::shared_ptr<Scene>> m_Scenes;
public:
	std::vector<std::shared_ptr<Scene>>& GetScenes() { return m_Scenes; };

	void LoadScene(std::string path);
	void UnloadScene(std::string path);

	void LoadGlobalScene(std::string path);
};