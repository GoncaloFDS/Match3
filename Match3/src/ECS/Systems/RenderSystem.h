#pragma once
#include "ECS/System.h"
#include "ECS/Components/Sprite.h"
#include "ECS/Components/Position.h"
#include "Game.h"

struct RenderNode {
	Sprite* sprite;
	Position* position;

	RenderNode(Sprite* s, Position* p) : sprite(s), position(p) {}
};

class RenderSystem : public System {

public:
	bool OnStart(SDL_Window* window, const std::string& title, int width, int height, bool fullscreen) {
		int flags = 0;
		if(fullscreen)
			flags = SDL_WINDOW_FULLSCREEN;

		if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			LOG_ERROR("SDL Failed to Initialize");
			return false;
		}

		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		                          width, height, flags);
		if(window)
			LOG_INFO("Window Created");

		Game::renderer = SDL_CreateRenderer(window, -1, 0);
		if(Game::renderer) {
			SDL_SetRenderDrawColor(Game::renderer, 255, 255, 0, 255);
			LOG_INFO("Renderer Created");
		}

		return true;
	}

	void OnUpdate() override {
		SDL_RenderClear(Game::renderer);

		for(RenderNode* node : m_targets) {
			//OnUpdate Sprite with the Position
			SDL_Rect dstrect{
				node->position->x(), node->position->y(),
				40, 40
			};
			SDL_RenderCopy(Game::renderer, node->sprite->GetTexture(), nullptr, &dstrect);
		}

		SDL_RenderPresent(Game::renderer);
	}

	void CreateNode(Entity* entity) override {
		if(entity->ContainsComponent<Sprite>() &&
			entity->ContainsComponent<Position>()) {
			auto* node = new RenderNode(&entity->GetComponent<Sprite>(),
			                            &entity->GetComponent<Position>());
			m_targets.push_back(node);
		}

	}

private:
	std::vector<RenderNode*> m_targets;
};
