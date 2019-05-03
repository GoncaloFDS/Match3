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

		node->color = static_cast<JewelColor>(Random::get(0, 4)); // get a random color
		node->sprite->SetTexture(TextureManager::GetCachedTexture(node->color));
		node->tilePosition->x = x;
		node->tilePosition->y = y;
		m_tiles[x][y] = std::move(node);
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

		LOG_INFO("Swaping tile [{0}][{1}] with tile [{2}][{3}]",
		         m_tile1->tilePosition->x, m_tile1->tilePosition->y,
		         m_tile2->tilePosition->x, m_tile2->tilePosition->y);

		const int tempA = m_tile1->position->x;
		const int tempB = m_tile1->position->y;
		const int tempC = m_tile1->tilePosition->x;
		const int tempD = m_tile1->tilePosition->y;

		m_tile1->position->x = m_tile2->position->x;
		m_tile1->position->y = m_tile2->position->y;
		m_tile1->tilePosition->x = m_tile2->tilePosition->x;
		m_tile1->tilePosition->y = m_tile2->tilePosition->y;

		m_tile2->position->x = tempA;
		m_tile2->position->y = tempB;
		m_tile2->tilePosition->x = tempC;
		m_tile2->tilePosition->y = tempD;

		m_tile1->sprite->alpha = 255;
		m_tile2->sprite->alpha = 255;

		m_tile1 = m_tile2 = nullptr;
	}
}

void TileSystem::SelectTile(bool isKeyDownEvent) {
	auto [mouseX, mouseY] = Input::GetMousePos();

	for(int x = 0; x < m_horizontalTiles; x++) {
		for(int y = 0; y < m_verticalTiles; y++) {
			auto& node = m_tiles[x][y];
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

}
