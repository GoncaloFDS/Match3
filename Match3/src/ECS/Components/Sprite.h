#pragma once
#include "ECS/Component.h"
#include "SDL.h"
#include "TextureManager.h"

class Sprite : public Component {
public:
	Sprite() = default;
	Sprite(const std::string& path) {
		LoadTexture(path);
	}

	Sprite(SDL_Texture* texture) {
		SetTexture(texture);
	}

	void LoadTexture(const std::string& path) {
		m_texture = TextureManager::LoadTexture(path);
	}

	void SetTexture(SDL_Texture* texture) {
		m_texture = texture;
	}

	SDL_Texture* GetTexture() const {
		return m_texture;
	}

private:
	SDL_Texture* m_texture = nullptr;
};
