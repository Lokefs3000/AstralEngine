#pragma once

#include "Variables.h"

#include "components/EntityComponents.h"

#include <vector>
#include <string>

class EXPORT Scene {
private:
	std::vector<Entity> m_GameEntities;
	std::vector<std::shared_ptr<EntityComponents>> m_Components;

	void ReorderEntities();
public:
	std::string Name = "";

	std::vector<Entity>& GetEntities() { return m_GameEntities; }
	std::vector<std::shared_ptr<EntityComponents>>& GetComponents() { return m_Components; }

	std::shared_ptr<EntityComponents> GetComponents(Entity& entity);

	Entity& AddEntity();
	void RemoveEntity(Entity entity);
};