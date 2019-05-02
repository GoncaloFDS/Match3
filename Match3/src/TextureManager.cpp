#include "pch.h"
#include "TextureManager.h"
#include "Game.h"
#include <SDL_image.h>

std::map<int, SDL_Texture*> TextureManager::m_cachedTextures;

SDL_Texture* TextureManager::LoadTexture(const std::string& fileName) {
	SDL_Surface* tempSurface = IMG_Load(fileName.c_str());
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return tex;
}

SDL_Texture* TextureManager::CacheTexture(const std::string& fileName, int key) {
	auto* tex = LoadTexture(fileName);
	m_cachedTextures[key] = tex;
	return tex;
}

SDL_Texture* TextureManager::GetCachedTexture(int key) {
	return m_cachedTextures[key];
}


