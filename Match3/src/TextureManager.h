#pragma once

#include "SDL.h"
#include <string>

class TextureManager {
	
public:
	static SDL_Texture* LoadTexture(const std::string& fileName);
	static SDL_Texture* CacheTexture(const std::string& fileName, int key);
	static SDL_Texture* GetCachedTexture(int key);

private:
	static std::map<int, SDL_Texture*> m_cachedTextures;
};
