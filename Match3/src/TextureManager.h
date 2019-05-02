#pragma once

#include "SDL.h"
#include <string>

class TextureManager {
	
public:
	static SDL_Texture* LoadTexture(const std::string& fileName);
};
