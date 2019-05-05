#include "pch.h"

#include "SDL.h"
#include "Game.h"

int main(int argc, char* argv[]) {
	std::unique_ptr<Game> game = std::make_unique<Game>("Match3", 800, 640, false);

	const int targetFrameRate = 144;
	const int targetFrameDuration = 1000 / targetFrameRate;

	Timer::Start();
	while(game->IsRunning()) {
		const Uint32 frameStart = SDL_GetTicks();
		game->OnEvent();
		game->OnUpdate();
		
		const Uint32 frameDuration = SDL_GetTicks() - frameStart;
		if(targetFrameDuration > frameDuration)
			SDL_Delay(targetFrameDuration- frameDuration);
		
		Timer::Tick();
	}

	return 0;
}
