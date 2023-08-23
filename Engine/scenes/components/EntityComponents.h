#pragma once

#include "Variables.h"

#include "Component.h"

#include <vector>
#include <memory>
#include <type_traits>

struct EntityComponents {
private:
	std::vector<std::shared_ptr<Component>> m_Components;
public:
	//https://gamedev.stackexchange.com/questions/58887/entity-system-creating-components
	template<typename T, typename... TArgs>
	T& AddComponent(TArgs&&... mArgs);

	template<typename T>
	void RemoveComponent();

	template<typename T>
	T* GetComponent();
};

template<typename T, typename ...TArgs>
inline T& EntityComponents::AddComponent(TArgs && ...mArgs)
{
	static_assert(std::is_base_of_v<Component, T>, "AddComponent requires specified type being a base clas of Component");

	auto result = std::make_shared<Component>(std::forward<TArgs>(mArgs)...);
	m_Components.push_back(result);
	return *result.get();
}

template<typename T>
inline void EntityComponents::RemoveComponent()
{
	static_assert(std::is_base_of_v<Component, T>, "RemoveComponent requires specified type being a base clas of Component");

	for (size_t i = 0; i < m_Components.size(); i++)
		if (m_Components[i]->GetComponentId() == T::GetComponentId()) {
			m_Components.erase(m_Components.begin() + i);
			break;
		}
}

template<typename T>
inline T* EntityComponents::GetComponent()
{
	static_assert(std::is_base_of_v<Component, T>, "GetComponent requires specified type being a base clas of Component");

	for (size_t i = 0; i < m_Components.size(); i++)
		if (m_Components[i]->GetComponentId() == T::GetComponentId())
			return (T*)m_Components[i].get();

	return NULL;
}
