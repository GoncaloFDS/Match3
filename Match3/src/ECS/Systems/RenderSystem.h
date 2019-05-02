#pragma once
#include "ECS/System.h"
#include "ECS/Components/Sprite.h"
#include "ECS/Components/Position.h"
#include "Game.h"

struct RenderNode {
	Sprite* sprite;
	Position* position;
};

class RenderSystem : public System {

public:

	void Update() override {
		SDL_RenderClear(Game::renderer);

		for(RenderNode* node : m_targets) {
			//Update Sprite with the Position
			
			
			SDL_RenderCopy(Game::renderer, node->sprite->GetTexture(), nullptr, nullptr);
		}
		
		SDL_RenderPresent(Game::renderer);
	}

	void AddTarget(RenderNode* node) {
		m_targets.push_back(node);
	}

private:
	std::vector<RenderNode*> m_targets;
};
