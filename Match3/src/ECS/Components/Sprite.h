#pragma once
#include "ECS/Component.h"
#include "SDL.h"
#include "TextureManager.h"

class Sprite : public Component {
public:
	Sprite() = default;
	Sprite(const std::string& path) {
		SetTexture(path);
	}

	void SetTexture(const std::string& path) {
		m_texture = TextureManager::LoadTexture(path);
	}

	SDL_Texture* GetTexture() const {
		return m_texture;
	}

private:
	SDL_Texture* m_texture = nullptr;
};
