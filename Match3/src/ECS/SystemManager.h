#pragma once

#include "pch.h"
#include "System.h"

class SystemManager {
public:
	void OnUpdate() {
		for(auto& s : m_systems)
			s->OnUpdate();
	}

	void OnEvent(SDL_Event& event) {
		for(auto& s : m_systems)
			s->OnEvent(event);
	}

	/**
	 * \brief Creates a new System of type T and calls the OnStart method
	 * \tparam T System derived class
	 * \tparam TArgs Match correspondent OnStart method arguments 
	 * \param mArgs arguments passed to the OnStart method of the given System
	 */
	template <typename T, typename ... TArgs>
	void AddSystem(TArgs&& ... mArgs) {
		std::unique_ptr<T> system = std::make_unique<T>();
		m_systems.push_back(std::move(system));
		T* s = static_cast<T*>(m_systems.back().get());
		s->OnStart(std::forward<TArgs>(mArgs)...);
	}

	/**
	 * \brief Generates necessary nodes for each System from the given Entity
	 * \param entity
	 */
	void CreateNodes(Entity* entity) {
		for(auto& s : m_systems)
			s->CreateNode(entity);
	}

private:
	std::vector<std::unique_ptr<System>> m_systems;
};
