#include "pch.h"
#include <iostream>

#include "SDL.h"
#include "Game.h"

int main(int argc, char* argv[]) {
	Game* game = new Game();
	game->Init("Match3", 800, 640, false);

	while(game->IsRunning()) {
		game->HandleEvents();
		game->Update();
		game->Render();
	}

	game->Clean();

	return 0;
}
