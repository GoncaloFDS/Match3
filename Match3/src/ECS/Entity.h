#pragma once
#include "pch.h"
#include "Component.h"
#include "ECS/Components/Size.h"

class Entity {
public:
	Entity(const int tag = 0) : tag(tag) {}

	template <typename T>
	bool ContainsComponent() const {
		return m_components.count(GetComponentTypeId<T>()) != 0;
	}

	template <typename T, typename ... TArgs>
	void AddComponent(TArgs&&... mArgs) {
		std::unique_ptr<T> component = std::make_unique<T>(std::forward<TArgs>(mArgs)...);
		component->entity = this;
		m_components[GetComponentTypeId<T>()] = std::move(component);
	}

	template <typename T>
	T& GetComponent() {
		return *static_cast<T*>(m_components[GetComponentTypeId<T>()].get());
	}

	template <typename T>
	void RemoveComponent() {
		m_components.erase(GetComponentTypeId<T>());
	}

	int tag; // eg. jewel, player, camera...defined by the user

private:
	std::unordered_map<ComponentId, std::unique_ptr<Component>> m_components;
};
