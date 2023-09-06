#include "pch.h"
#include "Scene.h"

#include "GameObject.h"

#include "components/Transform.h"

void Scene::ReorderEntities()
{
    for (size_t i = 0; i < m_GameEntities.size(); i++)
    {
        m_GameEntities[i]->Id = i;
    }
}

std::shared_ptr<EntityComponents> Scene::GetEntityComponents(std::shared_ptr<GameObject> entity)
{
    if (entity->Id >= m_Components.size())
        return NULL;
    return m_Components[entity->Id];
}

std::shared_ptr<GameObject> Scene::AddEntity()
{
    std::shared_ptr<GameObject> id = std::make_shared<GameObject>();
    id->Id = m_GameEntities.size();
    id->Components = std::make_shared<EntityComponents>();

    id->Components->AddComponent<Transform>();

    m_GameEntities.push_back(id);
    m_Components.push_back(id->Components);

    return id;
}

void Scene::RemoveEntity(std::shared_ptr<GameObject> entity)
{
    auto find = std::find(m_GameEntities.begin(), m_GameEntities.end(), entity);
    if (find != m_GameEntities.end()) {
        m_GameEntities[find - m_GameEntities.begin()]->SetParent(m_GameEntities[find - m_GameEntities.begin()], NULL);

        m_GameEntities.erase(find);
        m_Components.erase(m_Components.begin() + (find - m_GameEntities.begin()));
        
        ReorderEntities();
    }
}
