#pragma once

#include "pch.h"
#include "System.h"

class SystemManager {
public:
	void AddSystem(System* system) {
		m_systems.push_back(system);
		//system.Init()
	}

	void Update() {
		for(System* s : m_systems)
			s->Update();
	}

	template <typename T, typename ... TArgs>
	T* AddSystem(TArgs&& ... mArgs) {
		T* system = new T(std::forward<TArgs>(mArgs)...);
		m_systems.push_back(system);

		return system;
	}

	void CreateNodes(Entity* entity) {
		for(System* s : m_systems)
			s->CreateNode(entity);
	}

private:
	std::vector<System*> m_systems;
};
