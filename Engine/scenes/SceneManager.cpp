#include "pch.h"
#include "SceneManager.h"

#include "Scene.h"

void SceneManager::LoadScene(std::string path)
{
	m_Scenes.push_back(std::make_shared<Scene>());
}

void SceneManager::UnloadScene(std::string path)
{
}

void SceneManager::LoadGlobalScene(std::string path)
{
}
