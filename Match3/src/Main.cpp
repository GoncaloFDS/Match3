#include "pch.h"

#include "SDL.h"
#include "Game.h"

int main(int argc, char* argv[]) {
	std::unique_ptr<Game> game = std::make_unique<Game>();

	game->Init("Match3", 800, 640, false);

	while(game->IsRunning()) {
		game->HandleEvents();
		game->Update();
	}

	game->Clean();
	
	return 0;
}
