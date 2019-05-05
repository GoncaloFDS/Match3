#include "pch.h"
#include "RenderSystem.h" 
#include "Game.h"
#include "ECS/Components/Sprite.h"
#include "ECS/Components/Size.h"
#include "ECS/Components/Position.h"

void RenderSystem::OnStart(SDL_Window* window, const std::string& title, int width, int height, bool fullscreen) {
	int flags = 0;
	if(fullscreen)
		flags = SDL_WINDOW_FULLSCREEN;

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		LOG_ERROR("SDL Failed to Initialize");
		return;
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

	SDL_GL_SetSwapInterval(1);

}

void RenderSystem::OnUpdate() {
	SDL_RenderClear(Game::renderer);

	for(auto& node : m_targets) {
		if(node->size->fullscreen) {
			SDL_RenderCopy(Game::renderer, node->sprite->GetTexture(), nullptr, nullptr);
		}
		else {

			SDL_Rect dstrect{
				node->position->x, node->position->y,
				node->size->w, node->size->h
			};
			SDL_SetTextureAlphaMod(node->sprite->GetTexture(), node->sprite->alpha);
			SDL_RenderCopy(Game::renderer, node->sprite->GetTexture(), nullptr, &dstrect);
			SDL_SetTextureAlphaMod(node->sprite->GetTexture(), 255);
		}
	}

	SDL_RenderPresent(Game::renderer);


}

void RenderSystem::CreateNode(Entity* entity) {
	if(entity->tag == Tag::Jewel || entity->tag == Tag::Background) {
		auto node = std::make_unique<RenderNode>(&entity->GetComponent<Sprite>(),
		                                         &entity->GetComponent<Position>(),
		                                         &entity->GetComponent<Size>());
		m_targets.push_back(std::move(node));
	}

}

void RenderSystem::OnEvent(SDL_Event& event) {}
