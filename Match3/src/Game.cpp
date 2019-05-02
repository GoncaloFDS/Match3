#include "pch.h"
#include "Game.h"
#include "TextureManager.h"
#include "ECS/Entity.h"
#include "ECS/Components/Position.h"
#include "ECS/Components/Sprite.h"
#include "ECS/Systems/RenderSystem.h"

Entity* jewel;

SDL_Renderer* Game::renderer = nullptr;
RenderSystem renderSystem;
RenderNode* jewelNode;

Game::Game() {}

Game::~Game() {}

void Game::Init(const std::string& title, int width, int height, bool fullscreen) {

	Log::Init();
	LOG_INFO("Initialized Log!");

	int flags = 0;
	if(fullscreen)
		flags = SDL_WINDOW_FULLSCREEN;

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		LOG_ERROR("SDL Failed to Initialize");
		m_isRunning = false;
		return;
	}

	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
	if(m_window)
		LOG_INFO("Window Created");

	renderer = SDL_CreateRenderer(m_window, -1, 0);
	if(renderer) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		LOG_INFO("Renderer Created");
	}

	m_isRunning = true;

	jewel = new Entity();
	auto& p = jewel->AddComponent<Position>();
	auto& s = jewel->AddComponent<Sprite>("assets/Color-1.png");
	jewelNode = new RenderNode();
	jewelNode->position = &p;
	jewelNode->sprite = &s;

	renderSystem.AddTarget(jewelNode);
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
}

void Game::Render() {
	renderSystem.Update();

}

void Game::Clean() {
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	LOG_INFO("Game Context Destroyed");
}
