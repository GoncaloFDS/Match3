#include "pch.h"
#include "Game.h"
#include "TextureManager.h"
#include "ECS/Entity.h"
#include "ECS/Components/Position.h"
#include "ECS/Components/Sprite.h"
#include "ECS/Systems/RenderSystem.h"
#include "Input.h"
#include "ECS/SystemManager.h"
#include "ECS/Systems/TileSystem.h"
#include "random.h"

SDL_Renderer* Game::renderer = nullptr;

void Game::Init(const std::string& title, int width, int height, bool fullscreen) {

	Log::Init();
	LOG_INFO("Initialized Log!");
	//Create systems
	auto* tileSystem = m_systemManager.AddSystem<TileSystem>();
	auto* renderSystem = m_systemManager.AddSystem<RenderSystem>();
	m_isRunning = renderSystem->OnStart(m_window, title, width, height, fullscreen);

	//Cache Textures
	TextureManager::CacheTexture("assets/Color-1.png", 0);
	TextureManager::CacheTexture("assets/Color-2.png", 1);
	TextureManager::CacheTexture("assets/Color-3.png", 2);
	TextureManager::CacheTexture("assets/Color-4.png", 3);
	TextureManager::CacheTexture("assets/Color-5.png", 4);

	//Create GameObjects
	const int xBorder = 200;
	const int yBorder = 120;
	for(int x = 0; x < 8; x++) {
		for(int y = 0; y < 8; y++) {
			std::unique_ptr<Entity> jewel = std::make_unique<Entity>();
			jewel->AddComponent<Position>(xBorder + x * 50, yBorder + y * 50);
			jewel->AddComponent<Sprite>(TextureManager::GetCachedTexture(Random::get(0, 4)));
			m_systemManager.CreateNodes(jewel.get());
			m_entities.push_back(std::move(jewel)); 
		}
	}

}

void Game::OnEvent() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {

		switch(event.type) {
		case SDL_QUIT:
			m_isRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			LOG_INFO("button down");
			break;
		default:
			break;
		}
	}
}

void Game::OnUpdate() {
	Input::GetMousePos();
	m_systemManager.OnUpdate();
}

void Game::Clean() {
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	LOG_INFO("Game Context Destroyed");
}
