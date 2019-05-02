#include "pch.h"
#include "TextureManager.h"
#include "Game.h"
#include <SDL_image.h>

SDL_Texture* TextureManager::LoadTexture(const std::string& fileName) {
	SDL_Surface* tempSurface = IMG_Load(fileName.c_str());
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return tex;
}


