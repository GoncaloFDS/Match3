#include "pch.h"
#include "TileSystem.h"
#include "Game.h"
#include "Input.h"
#include "TextureManager.h"
#include "SDL.h"
#include "ECS/Components/Sprite.h"
#include "ECS/Components/Transform.h"

float swapDuration = 0.5f;
float collapseDuration = 0.8f;
int totalPoints = 0;

void TileSystem::OnStart(int horizontalTiles, int verticalTiles) {
	m_horizontalTiles = horizontalTiles;
	m_verticalTiles = verticalTiles;
	m_grid.resize(horizontalTiles);
	for(auto& row : m_grid)
		row.resize(verticalTiles);

	for(int x = 0; x < horizontalTiles; x++) {
		for(int y = 0; y < horizontalTiles; y++) {
			Entity* tile = Game::CreateEntity(Tag::Jewel);
			vec2 pos(m_xBorder + x * m_innerBorder, m_yBorder - y * m_innerBorder);
			vec2 size(40, 40);
			tile->AddComponent<Transform>(pos, size);
			tile->AddComponent<Sprite>();
			tile->AddComponent<Tile>();
			tile->AddComponent<Animation>();
		}
	}

	Game::state = State::Input;
}

void TileSystem::OnUpdate() {

	int points = 0;
	switch(Game::state) {

	case State::Match:
		for(int x = 0; x < m_horizontalTiles; x++) {
			for(int y = 0; y < m_verticalTiles; y++) {
				points += MatchAt(x, y, m_grid[x][y]->tile->color);
			}
		}
		Game::state = points == 0 ? State::Input : State::Destroy;
		totalPoints += points;
		break;

	case State::Destroy:
		for(int x = 0; x < m_horizontalTiles; x++) {
			for(int y = 0; y < m_verticalTiles; y++) {
				if(m_grid[x][y]->tile->isMatched)
					StartDestroyAnimation(m_grid[x][y].get());
			}
		}
		Game::state = State::Wait;
		break;

	case State::Collapse:
		CollapseColumns();
		Game::state = State::Wait;
		break;

	case State::Refill:
		RefillTiles();
		Game::state = State::Wait;
		break;

	default: ;
	}
}

void TileSystem::OnEvent(SDL_Event& event) {
	switch(event.type) {
	case SDL_MOUSEBUTTONDOWN:
		if(Input::IsMouseButtonPressed(SDL_BUTTON(SDL_BUTTON_LEFT)) && Game::state == State::Input)
			SelectTile(true);
		break;
	case SDL_MOUSEBUTTONUP:
		if(event.button.button == SDL_BUTTON(SDL_BUTTON_LEFT) && Game::state == State::Input) {
			SelectTile(false);
			//Swap Selected Tiles if they are adjacent to each other
			if(m_tile1 && m_tile2 && (m_tile1->tile->pos - m_tile2->tile->pos).Quadrance() <= 1) {
				SwapTiles(m_tile1, m_tile2, AnimationState::Swapping);
				m_tile1 = m_tile2 = nullptr;
			}
		}
		break;
	case SDL_KEYDOWN:
		LOG_ERROR("Current state: {0}", static_cast<int>(Game::state));
		LOG_INFO("Points: {0}", totalPoints);
		break;
	default: ;
	}
}

void TileSystem::CreateNode(Entity* entity) {
	static int x = 0, y = 0;
	if(entity->tag == Tag::Jewel) {
		auto node = std::make_unique<TileNode>(&entity->GetComponent<Sprite>(),
		                                       &entity->GetComponent<Transform>(),
		                                       &entity->GetComponent<Tile>(),
		                                       &entity->GetComponent<Animation>());

		do node->tile->color = static_cast<JewelColor>(Random::get(0, 4)); // get a random color
		while(CheckMatchAt(x, y, node->tile->color)); // prevent matches on the first board

		node->sprite->SetTexture(TextureManager::GetCachedTexture(node->tile->color));
		node->tile->pos = vec2(x, y);
		m_grid[x][y] = std::move(node);
		if(++y >= m_verticalTiles) {
			y = 0;
			x++;
		}
	}
}

bool TileSystem::IsInsideRect(int x, int y, SDL_Rect& rect) {
	return x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h;
}

vec2 TileSystem::NodeToPixel(TileNode* node) const {
	return vec2(m_xBorder + node->tile->pos.x * m_innerBorder, m_yBorder - node->tile->pos.y * m_innerBorder);
}

void TileSystem::StartDestroyAnimation(TileNode* tile) const {
	tile->animation->elapsedTime = 0.0f;
	tile->animation->isPlaying = true;
	tile->animation->state = AnimationState::Destroying;
	tile->animation->duration = .5f;
	tile->animation->startAlpha = 255;
	tile->animation->endAlpha = 0;
	tile->animation->endPos = tile->animation->startPos;
}

void TileSystem::StartRefillAnimation(TileNode* tile) const {
	tile->animation->elapsedTime = 0.0f;
	tile->animation->isPlaying = true;
	tile->animation->state = AnimationState::Refilling;
	tile->animation->duration = .5f;
	tile->animation->startAlpha = 0;
	tile->animation->endAlpha = 255;
	tile->animation->endPos = tile->animation->startPos;
}

void TileSystem::StartSwapAnimation(TileNode* tile1, TileNode* tile2, AnimationState state) const {
	tile1->animation->startPos = tile1->transform->pos;
	tile1->animation->endPos = NodeToPixel(tile2);
	tile1->animation->elapsedTime = 0.0f;
	tile1->animation->isPlaying = true;
	tile1->animation->state = state;
	tile1->animation->duration = state == AnimationState::Swapping ? swapDuration : collapseDuration;
	tile1->animation->endAlpha = tile1->animation->startAlpha;
}

void TileSystem::SwapTiles(TileNode* tile1, TileNode* tile2, AnimationState state) {

	StartSwapAnimation(tile1, tile2, state);
	StartSwapAnimation(tile2, tile1, state);
	std::swap(tile1->tile->pos, tile2->tile->pos);

	// Swap grid pointers 
	m_grid[tile1->tile->pos.x][tile1->tile->pos.y].swap(
		m_grid[tile2->tile->pos.x][tile2->tile->pos.y]);

	if(state == AnimationState::Swapping) {
		tile1->sprite->alpha = 255;
	}
}

void TileSystem::SelectTile(bool isKeyDownEvent) {
	auto [mouseX, mouseY] = Input::GetMousePos();
	for(int x = 0; x < m_horizontalTiles; x++) {
		for(int y = 0; y < m_verticalTiles; y++) {
			auto& node = m_grid[x][y];
			SDL_Rect rect{
				static_cast<int>(node->transform->pos.x),
				static_cast<int>(node->transform->pos.y),
				static_cast<int>(node->transform->size.x),
				static_cast<int>(node->transform->size.y),
			};
			if(IsInsideRect(mouseX, mouseY, rect)) {
				if(m_tile1 == nullptr && isKeyDownEvent) {
					node->sprite->alpha = 100;
					m_tile1 = node.get();
				}
				else if(m_tile1 && m_tile1 != node.get()) {
					m_tile2 = node.get();
				}
				else if(isKeyDownEvent) {
					m_tile1->sprite->alpha = 255;
					m_tile1 = m_tile2 = nullptr;
				}
				return;
			}
		}
	}
}

bool TileSystem::CheckMatchAt(int x, int y, JewelColor color) {
	if(x > 1 && m_grid[x - 1][y]->tile->color == color && m_grid[x - 2][y]->tile->color == color)
		return true;
	if(y > 1 && m_grid[x][y - 1]->tile->color == color && m_grid[x][y - 2]->tile->color == color)
		return true;
	return false;
}

int TileSystem::MatchAt(int x, int y, JewelColor color) {
	int points = 0;
	if(x > 1 && m_grid[x - 1][y]->tile->color == color && m_grid[x - 2][y]->tile->color == color) {
		m_grid[x][y]->tile->isMatched = true;
		m_grid[x - 1][y]->tile->isMatched = true;
		m_grid[x - 2][y]->tile->isMatched = true;
		points++;
	}
	if(y > 1 && m_grid[x][y - 1]->tile->color == color && m_grid[x][y - 2]->tile->color == color) {
		m_grid[x][y]->tile->isMatched = true;
		m_grid[x][y - 1]->tile->isMatched = true;
		m_grid[x][y - 2]->tile->isMatched = true;
		points++;
	}
	return points == 2 ? points * 2 : points;
}

void TileSystem::CollapseColumns() {
	bool collapsed = false;
	for(int x = 0; x < m_horizontalTiles; x++) {
		for(int y = 0; y < m_verticalTiles; y++) {
			if(m_grid[x][y]->tile->isMatched) {
				for(int k = y + 1; k < m_verticalTiles; k++) {
					if(!m_grid[x][k]->tile->isMatched) {
						SwapTiles(m_grid[x][y].get(), m_grid[x][k].get(), AnimationState::Collapsing);
						collapsed = true;
						break;
					}
				}
				// fixes edge case where the top tiles have been destroyed
				// TODO: make a more elegant solution
				if(!collapsed && y == m_verticalTiles - 1)
					SwapTiles(m_grid[x][y].get(), m_grid[x][y].get(), AnimationState::Collapsing);

			}
		}
	}

}

void TileSystem::RefillTiles() {
	for(int x = 0; x < m_horizontalTiles; x++) {
		for(int y = 0; y < m_verticalTiles; y++) {
			auto& node = m_grid[x][y];
			if(node->tile->isMatched) {

				do node->tile->color = static_cast<JewelColor>(Random::get(0, 4)); // get a random color
				while(CheckMatchAt(x, y, node->tile->color)); // prevent matches on the first board

				node->sprite->SetTexture(TextureManager::GetCachedTexture(node->tile->color));
				node->tile->isMatched = false;
				StartRefillAnimation(node.get());
			}
		}
	}
}
