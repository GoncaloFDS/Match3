#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include <iostream>

class Game {

public:
	Game();
	~Game();

	void Init(const std::string& title,
	          int width, int height, bool fullscreen);

	void HandleEvents();
	void Update();
	void Render();
	void Clean();

	bool IsRunning() const { return m_isRunning; }

	static SDL_Renderer* renderer;
	
private:
	bool m_isRunning = false;
	SDL_Window* m_window;
};
