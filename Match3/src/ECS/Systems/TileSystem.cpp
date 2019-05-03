#include "pch.h"
#include "TileSystem.h"
#include "Game.h"
#include "Input.h"
#include "TextureManager.h"
#include "SDL.h"
#include "ECS/Components/Position.h"
#include "ECS/Components/Size.h"
#include "ECS/Components/Sprite.h"

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

		}
	}

}

void TileSystem::SelectTile() {
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
				node->sprite->alpha = 100;
				LOG_INFO("Tile: x {0} y {1}", x, y);
				return;
			}
		}
	}

}

void TileSystem::OnUpdate() {}

void TileSystem::OnEvent(SDL_Event& event) {
	switch(event.type) {
	case SDL_MOUSEBUTTONDOWN:
		LOG_INFO("Mouse Button Down");
		SelectTile();
		break;
	default: ;
	}
}

void TileSystem::CreateNode(Entity* entity) {
	static int x = 0, y = 0;
	if(entity->tag == Tag::Jewel) {
		auto node = std::make_unique<TileNode>(&entity->GetComponent<Sprite>(),
		                                       &entity->GetComponent<Position>(),
		                                       &entity->GetComponent<Size>());

		node->color = static_cast<JewelColor>(Random::get(0, 4)); // get a random color
		node->sprite->SetTexture(TextureManager::GetCachedTexture(node->color));
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
