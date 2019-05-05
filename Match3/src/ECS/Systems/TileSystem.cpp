#include "pch.h"
#include "TileSystem.h"
#include "Game.h"
#include "Input.h"
#include "TextureManager.h"
#include "SDL.h"
#include "ECS/Components/Sprite.h"
#include "ECS/Components/Transform.h"

void TileSystem::OnStart(int horizontalTiles, int verticalTiles) {
	m_horizontalTiles = horizontalTiles;
	m_verticalTiles = verticalTiles;
	m_grid.resize(horizontalTiles);
	for(auto& row : m_grid)
		row.resize(verticalTiles);

	for(int x = 0; x < horizontalTiles; x++) {
		for(int y = 0; y < horizontalTiles; y++) {
			Entity* tile = Game::CreateEntity(Tag::Jewel);
			vec2 pos(xBorder + x * innerBorder, yBorder - y * innerBorder);
			vec2 size(40, 40);
			tile->AddComponent<Transform>(pos, size);
			tile->AddComponent<Sprite>();
			tile->AddComponent<Tile>();
			tile->AddComponent<Animation>();
		}
	}

}

void TileSystem::OnUpdate() {
	//Swap Selected Tiles
	if(m_tile1 && m_tile2) {
		SwapTiles(m_tile1, m_tile2);
		m_tile1 = m_tile2 = nullptr;
	}

	//Look for matches
	for(int x = 0; x < m_horizontalTiles; x++) {
		for(int y = 0; y < m_verticalTiles; y++) {
			MatchAt(x, y, m_grid[x][y]->tile->color);
		}
	}

	//Destroy animation
	for(int x = 0; x < m_horizontalTiles; x++) {
		for(int y = 0; y < m_verticalTiles; y++) {
			if(m_grid[x][y]->tile->isMatched)
				m_grid[x][y]->sprite->alpha = 90;
		}
	}

	CollapseColumns();

	RespawnTiles();

	for(int x = 0; x < m_horizontalTiles; x++) {
		for(int y = 0; y < m_verticalTiles; y++) {
			if(m_grid[x][y]->animation->isPlaying) {
				auto& anim = m_grid[x][y]->animation;
				m_grid[x][y]->transform->pos += (anim->end - anim->start) * (Timer::delta_s / anim->duration);
				anim->elapsedTime += Timer::delta_s;
				if(anim->elapsedTime >= anim->duration)
					anim->isPlaying = false;
				break;
			}
		}
	}

}

void TileSystem::OnEvent(SDL_Event& event) {
	switch(event.type) {
	case SDL_MOUSEBUTTONDOWN:
		SelectTile(true);
		break;
	case SDL_MOUSEBUTTONUP:
		SelectTile(false);
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
		node->animation->duration = 1.0f;
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
	return vec2(xBorder + node->tile->pos.x * innerBorder, yBorder - node->tile->pos.y * innerBorder);
}

void TileSystem::StartSwapAnimation(TileNode* tile1, TileNode* tile2) const {
	tile1->animation->start = tile1->transform->pos;
	tile1->animation->end = NodeToPixel(tile2);
	tile1->animation->elapsedTime = 0;
	tile1->animation->isPlaying = true;
	tile2->animation->start = tile2->transform->pos;
	tile2->animation->end = NodeToPixel(tile1);
	tile2->animation->elapsedTime = 0;
	tile2->animation->isPlaying = true;
}

void TileSystem::SwapTiles(TileNode* tile1, TileNode* tile2) {
	if((tile1->tile->pos - tile2->tile->pos).Quadrance() <= 1) {

		StartSwapAnimation(tile1, tile2);
		std::swap(tile1->tile->pos, tile2->tile->pos);

		// Swap grid pointers 
		m_grid[tile1->tile->pos.x][tile1->tile->pos.y].swap(
			m_grid[tile2->tile->pos.x][tile2->tile->pos.y]);

	}
	tile1->sprite->alpha = 255;
	tile2->sprite->alpha = 255;
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
				LOG_INFO("Selecting x = {0}, y = {1}", node->tile->pos.x, node->tile->pos.y);
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
	return points;
}

void TileSystem::CollapseColumns() {
	for(int x = 0; x < m_horizontalTiles; x++) {
		for(int y = 0; y < m_verticalTiles; y++) {
			if(m_grid[x][y]->tile->isMatched) {
				for(int k = y + 1; k < m_verticalTiles; k++) {
					if(!m_grid[x][k]->tile->isMatched) {

						m_grid[x][k]->tile->isMatched = false;
						SwapTiles(m_grid[x][y].get(), m_grid[x][k].get());
						//break;
					}
				}
			}
		}
	}
}

void TileSystem::RespawnTiles() {}
