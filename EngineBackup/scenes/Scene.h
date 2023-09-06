#pragma once

#include "Variables.h"

#include "components/EntityComponents.h"

class GameObject;

#include <vector>
#include <string>

class EXPORT Scene {
private:
	std::vector<std::shared_ptr<GameObject>> m_GameEntities;
	std::vector<std::shared_ptr<EntityComponents>> m_Components;

	void ReorderEntities();
public:
	std::string Name = "";

	std::vector<std::shared_ptr<GameObject>>& GetEntities() { return m_GameEntities; }
	std::vector<std::shared_ptr<EntityComponents>>& GetComponents() { return m_Components; }

	std::shared_ptr<EntityComponents> GetEntityComponents(std::shared_ptr<GameObject> entity);

	std::shared_ptr<GameObject> AddEntity();
	void RemoveEntity(std::shared_ptr<GameObject> entity);
};