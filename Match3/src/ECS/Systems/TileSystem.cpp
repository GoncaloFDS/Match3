#include "pch.h"
#include "TileSystem.h"
#include "Game.h"
#include "Input.h"
#include "TextureManager.h"
#include "SDL.h"
#include "ECS/Components/Position.h"
#include "ECS/Components/Size.h"
#include "ECS/Components/Sprite.h"
#include "ECS/Components/TilePosition.h"

void TileSystem::OnStart(int horizontalTiles, int verticalTiles) {
	m_horizontalTiles = horizontalTiles;
	m_verticalTiles = verticalTiles;
	m_tiles.resize(horizontalTiles);
	for(auto& row : m_tiles)
		row.resize(verticalTiles);

	for(int x = 0; x < horizontalTiles; x++) {
		for(int y = 0; y < horizontalTiles; y++) {
			Entity* tile = Game::CreateEntity(Tag::Jewel);
			tile->AddComponent<Position>(xBorder + x * innerBorder, yBorder + y * innerBorder);
			tile->AddComponent<Size>(40, 40);
			tile->AddComponent<Sprite>();
			tile->AddComponent<TilePosition>();

		}
	}

}

void TileSystem::OnUpdate() {
	if(m_tile1 && m_tile2)
		SwapTiles();
	for(int x = 0; x < m_horizontalTiles; x++) {
		for(int y = 0; y < m_verticalTiles; y++) {
			if(MatchAt(x, y, m_tiles[x][y]))
				LOG_INFO("Match at {0}, {1}", x, y);
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
		                                       &entity->GetComponent<Position>(),
		                                       &entity->GetComponent<Size>(),
		                                       &entity->GetComponent<TilePosition>());

		do node->color = static_cast<JewelColor>(Random::get(0, 4)); // get a random color
		while(MatchAt(x, y, node->color)); // prevent matches on the first board

		node->sprite->SetTexture(TextureManager::GetCachedTexture(node->color));
		node->tilePosition->x = x;
		node->tilePosition->y = y;
		m_tiles[x][y] = node->color;
		m_targets.push_back(std::move(node));
		if(++y >= m_verticalTiles) {
			y = 0;
			x++;
		}

	}

}

bool TileSystem::IsInsideRect(int x, int y, SDL_Rect& rect) {
	return x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h;
}

void TileSystem::SwapTiles() {
	if(std::abs(m_tile1->tilePosition->x - m_tile2->tilePosition->x) +
		std::abs(m_tile1->tilePosition->y - m_tile2->tilePosition->y) <= 1) {

		std::swap(*m_tile1->position, *m_tile2->position);
		std::swap(*m_tile1->tilePosition, *m_tile2->tilePosition);

		m_tiles[m_tile1->tilePosition->x][m_tile1->tilePosition->y] = m_tile1->color;
		m_tiles[m_tile2->tilePosition->x][m_tile2->tilePosition->y] = m_tile2->color;
		

		m_tile1->sprite->alpha = 255;
		m_tile2->sprite->alpha = 255;

		m_tile1 = m_tile2 = nullptr;
	}
}

void TileSystem::SelectTile(bool isKeyDownEvent) {
	auto [mouseX, mouseY] = Input::GetMousePos();

	for(auto& node : m_targets){
			SDL_Rect rect{
				node->position->x,
				node->position->y,
				node->size->w,
				node->size->h,
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

bool TileSystem::MatchAt(int x, int y, JewelColor color) {
	if(x > 1 && m_tiles[x - 1][y] == color && m_tiles[x - 2][y] == color)
		return true;
	if(y > 1 && m_tiles[x][y - 1] == color && m_tiles[x][y - 2] == color)
		return true;
	return false;
}
