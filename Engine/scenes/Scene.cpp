#include "pch.h"
#include "Scene.h"

void Scene::ReorderEntities()
{
    for (size_t i = 0; i < m_GameEntities.size(); i++)
    {
        m_GameEntities[i] = i;
    }
}

std::shared_ptr<EntityComponents> Scene::GetComponents(Entity& entity)
{
    if (entity >= m_Components.size())
        return NULL;
    return m_Components[entity];
}

Entity& Scene::AddEntity()
{
    Entity id = m_GameEntities.size();

    m_GameEntities.push_back(id);
    m_Components.push_back(std::make_shared<EntityComponents>());

    return id;
}

void Scene::RemoveEntity(Entity entity)
{
    auto find = std::find(m_GameEntities.begin(), m_GameEntities.end(), entity);
    if (find != m_GameEntities.end()) {
        m_GameEntities.erase(find);
        m_Components.erase(m_Components.begin() + (find - m_GameEntities.begin()));
        
        ReorderEntities();
    }
}
