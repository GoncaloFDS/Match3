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

Entity* jewel;

SDL_Renderer* Game::renderer = nullptr;
SystemManager systemManager;

void Game::Init(const std::string& title, int width, int height, bool fullscreen) {

	Log::Init();
	LOG_INFO("Initialized Log!");
	//Create systems
	auto* tileSystem = systemManager.AddSystem<TileSystem>();
	auto* renderSystem = systemManager.AddSystem<RenderSystem>();
	m_isRunning = renderSystem->Init(m_window, title, width, height, fullscreen);

	//Create GameObjects
	jewel = new Entity(Jewel);
	jewel->AddComponent<Position>();
	jewel->AddComponent<Sprite>("assets/Color-3.png");
	m_entities.push_back(jewel);
	systemManager.CreateNodes(jewel);
	
}

void Game::HandleEvents() {
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

void Game::Update() {
	auto p = jewel->GetComponent<Position>();
	Input::GetMousePos();
	systemManager.Update();
}

void Game::Clean() {
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	LOG_INFO("Game Context Destroyed");
}
