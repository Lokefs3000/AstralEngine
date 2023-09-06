#pragma once

#include "Variables.h"

#include "Component.h"

#include <vector>
#include <memory>
#include <type_traits>
#include <string>

struct EntityComponents {
private:
	std::vector<std::shared_ptr<Component>> m_Components;
public:
	std::string Name = "New Entity";

	//https://gamedev.stackexchange.com/questions/58887/entity-system-creating-components
	template<typename T, typename... TArgs>
	T* AddComponent(TArgs&&... mArgs)
	{
		static_assert(std::is_base_of_v<Component, T>, "AddComponent requires specified type being a base clas of Component");

		auto result = std::make_shared<T>(std::forward<TArgs>(mArgs)...);
		m_Components.push_back(result);
		return (T*)result.get();
	}

	template<typename T>
	void RemoveComponent()
	{
		static_assert(std::is_base_of_v<Component, T>, "RemoveComponent requires specified type being a base clas of Component");

		for (size_t i = 0; i < m_Components.size(); i++)
			if (m_Components[i]->GetComponentId() == T::GetComponentId()) {
				m_Components.erase(m_Components.begin() + i);
				break;
			}
	}

	template<typename T>
	T* GetComponent()
	{
		static_assert(std::is_base_of_v<Component, T>, "GetComponent requires specified type being a base clas of Component");

		for (size_t i = 0; i < m_Components.size(); i++)
			if (m_Components[i]->GetComponentId() == T::GetComponentId())
				return (T*)m_Components[i].get();

		return NULL;
	}

	std::vector<std::shared_ptr<Component>>& GetComponents() { return m_Components; }
};