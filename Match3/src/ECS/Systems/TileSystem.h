#pragma once
#include "pch.h"
#include "ECS/System.h"
#include "ECS/Components/Animation.h"
#include "ECS/Components/Tile.h"

class Sprite;
class Entity;
struct Transform;
struct SDL_Rect;

struct TileNode {
	Sprite* sprite;
	Transform* transform;
	Tile* tile;
	Animation* animation;

	TileNode(Sprite* sp, Transform* trans, Tile* tp, Animation* anim) :
		sprite(sp), transform(trans), tile(tp), animation(anim) {}
};

class TileSystem : public System {

public:
	void OnStart(int horizontalTiles, int verticalTiles);

	void OnUpdate() override;
	void OnEvent(SDL_Event& event) override;

	void CreateNode(Entity* entity) override;

private:
	//std::vector<std::unique_ptr<TileNode>> m_targets;
	std::vector<std::vector<std::unique_ptr<TileNode>>> m_grid;

	const int xBorder = 200;
	const int yBorder = 460;
	const int innerBorder = 50;

	int m_horizontalTiles;
	int m_verticalTiles;

	TileNode* m_tile1 = nullptr;
	TileNode* m_tile2 = nullptr;

	bool IsInsideRect(int x, int y, SDL_Rect& rect);
	vec2 NodeToPixel(TileNode* node) const;
	void StartSwapAnimation(TileNode* tile1, TileNode* tile2) const;
	void SwapTiles(TileNode* tile1, TileNode* tile2);
	void SelectTile(bool isKeyDownEvent);
	bool CheckMatchAt(int x, int y, JewelColor color);
	int MatchAt(int x, int y, JewelColor color);
	void CollapseColumns();
	void RespawnTiles();
};
