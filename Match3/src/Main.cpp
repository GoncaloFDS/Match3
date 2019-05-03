#include "pch.h"

#include "SDL.h"
#include "Game.h"

int main(int argc, char* argv[]) {
	std::unique_ptr<Game> game = std::make_unique<Game>();

	game->Init("Match3", 800, 640, false);

	Timer::Start();
	while(game->IsRunning()) {
		game->OnEvent();
		game->OnUpdate();
		Timer::Tick();
	}

	game->Clean();
	
	return 0;
}
