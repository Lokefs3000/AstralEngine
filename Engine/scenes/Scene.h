#pragma once

#include "Variables.h"

#include <vector>
#include "components/ComponentContainer.h"

class EXPORT Scene {
private:
	std::vector<Entity> m_GameEntities;
	std::vector<ComponentContainer> m_Components;
public:
	std::vector<Entity>& GetEntities() { return m_GameEntities; }
	std::vector<ComponentContainer>& GetComponents() { return m_Components; }
};