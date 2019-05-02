#pragma once

#include "SDL.h"
#include <string>
#include <iostream>
#include "ECS/Entity.h"
#include "ECS/SystemManager.h"

class Game {

public:
	Game() = default;
	~Game() = default;

	void Init(const std::string& title,
	          int width, int height, bool fullscreen);

	void OnEvent();
	void OnUpdate();
	void Clean();

	bool IsRunning() const { return m_isRunning; }

	static SDL_Renderer* renderer;

private:
	bool m_isRunning = false;
	SDL_Window* m_window;
	SystemManager m_systemManager;
	std::vector<std::unique_ptr<Entity>> m_entities;
};


enum Tag {
	None = 0,
	Jewel
};