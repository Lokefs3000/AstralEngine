#pragma once

#include <set>
#include <vector>
#include <variant>
#include <string>
#include <memory>

struct AstralValue {
private:
	std::vector<std::variant<int, float, bool, std::string>> m_InternalData;
	bool m_IsListData = false;
public:
	AstralValue(std::string name, std::variant<int, float, bool, std::string> data);
	AstralValue(std::string name, std::vector<std::variant<int, float, bool, std::string>> data);

	std::string Name = "";

	template <typename T>
	bool Is();
	template <typename T>
	T Get();
	template <typename T>
	bool IsAt(uint32_t idx = 0);
	template <typename T>
	T GetAt(uint32_t idx = 0);
	uint32_t GetSize();
};

struct AstralObject
{
private:
	
public:
	AstralObject();

	std::string Id;

	std::vector<std::shared_ptr<AstralObject>> Children;
	std::vector<std::shared_ptr<AstralValue>> Values;

	std::shared_ptr<AstralObject> GetChild(std::string id);
	std::shared_ptr<AstralValue> GetValue(std::string id);
};

inline AstralObject::AstralObject()
{
}

AstralValue::AstralValue(std::string name, std::variant<int, float, bool, std::string> data)
{
	m_IsListData = false;
	m_InternalData.push_back(data);
	Name = name;
}

AstralValue::AstralValue(std::string name, std::vector<std::variant<int, float, bool, std::string>> data)
{
	m_IsListData = true;
	m_InternalData = data;
	Name = name;
}

std::shared_ptr<AstralObject> AstralObject::GetChild(std::string id)
{
	for (size_t i = 0; i < Children.size(); i++)
		if (Children[i]->Id == id)
			return Children[i];
	return NULL;
}

inline std::shared_ptr<AstralValue> AstralObject::GetValue(std::string id)
{
	for (size_t i = 0; i < Values.size(); i++)
		if (Values[i]->Name == id)
			return Values[i];
	return NULL;
}

template<typename T>
inline bool AstralValue::Is()
{
	if (m_IsListData)
		return false;
	return std::holds_alternative<T>(m_InternalData[0]);
}

template<typename T>
inline T AstralValue::Get()
{
	if (m_IsListData)
		return T();
	return std::get<T>(m_InternalData[0]);
}

template<typename T>
inline bool AstralValue::IsAt(uint32_t idx)
{
	if (!m_IsListData || m_InternalData.size() < idx)
		return false;
	return std::holds_alternative<T>(m_InternalData[idx]);
}

template<typename T>
inline T AstralValue::GetAt(uint32_t idx)
{
	if (!m_IsListData || m_InternalData.size() < idx)
		return T();
	return std::get<T>(m_InternalData[idx]);
}

inline uint32_t AstralValue::GetSize()
{
	return m_InternalData.size();
}
