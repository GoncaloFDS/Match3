#pragma once
#include "pch.h"
#include "Component.h"

class Entity {
public:
	Entity(const int tag = 0) : tag(tag) {}

	template <typename T>
	bool ContainsComponent() const {
		return m_components.count(GetComponentTypeId<T>()) != 0;
	}

	template <typename T, typename ... TArgs>
	T& AddComponent(TArgs&&... mArgs) {
		T* component = new T(std::forward<TArgs>(mArgs)...);
		m_components[GetComponentTypeId<T>()] = component;
		component->entity = this;
		return *component;
	}

	template <typename T>
	T& GetComponent() {
		return *static_cast<T*>(m_components[GetComponentTypeId<T>()]);
	}

	template <typename T>
	void RemoveComponent() {
		m_components.erase(GetComponentTypeId<T>());
	}

	int tag; // eg. jewel, player, camera...defined by the user

private:
	std::unordered_map<ComponentId, Component*> m_components;
};
