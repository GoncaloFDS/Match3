#include "pch.h"
#include "Game.h"
#include "TextureManager.h"
#include "ECS/Entity.h"
#include "ECS/Components/Sprite.h"
#include "ECS/Systems/RenderSystem.h"
#include "Input.h"
#include "ECS/SystemManager.h"
#include "ECS/Systems/TileSystem.h"
#include "random.h"
#include "ECS/Components/Transform.h"

SDL_Renderer* Game::renderer = nullptr;
std::vector<std::unique_ptr<Entity>> Game::m_entities;

Game::Game(const std::string& title, int width, int height, bool fullscreen) {

	Log::Init();
	LOG_INFO("Initialized Log!");
	//Create systems
	m_systemManager.AddSystem<TileSystem>(8, 8);
	m_systemManager.AddSystem<RenderSystem>(m_window, title, width, height, fullscreen);

	//Cache Textures
	TextureManager::CacheTexture("assets/Color-0.png", 0);
	TextureManager::CacheTexture("assets/Color-1.png", 1);
	TextureManager::CacheTexture("assets/Color-2.png", 2);
	TextureManager::CacheTexture("assets/Color-3.png", 3);
	TextureManager::CacheTexture("assets/Color-4.png", 4);

	//Create GameObjects
	auto background = std::make_unique<Entity>(Tag::Background);
	background->AddComponent<Transform>(true); // fullscreen
	background->AddComponent<Sprite>("assets/Backdrop13.jpg");
	m_entities.insert(m_entities.begin(), std::move(background));

	for(auto& e : m_entities)
		m_systemManager.CreateNodes(e.get());
}

void Game::OnEvent() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		m_systemManager.OnEvent(event); // propagate the event to every system

		switch(event.type) {
		case SDL_QUIT:
			m_isRunning = false;
			break;
		default:
			break;
		}
	}
}

void Game::OnUpdate() {
	m_systemManager.OnUpdate();
	//SDL_Delay(1600);
}

Game::~Game() {
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	LOG_INFO("Game Context Destroyed");
}

Entity* Game::CreateEntity(Tag tag) {

	std::unique_ptr<Entity> e = std::make_unique<Entity>(tag);
	m_entities.push_back(std::move(e));
	return m_entities.back().get();
}

