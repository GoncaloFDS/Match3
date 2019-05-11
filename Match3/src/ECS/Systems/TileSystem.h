#pragma once
#include "pch.h"
#include "ECS/System.h"
#include "ECS/Components/Animation.h"
#include "ECS/Components/Tile.h"
#include "vec2.h"

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
	std::vector<std::vector<std::unique_ptr<TileNode>>> m_grid;

	const float m_xBorder = 200;
	const float m_yBorder = 460;
	const float m_innerBorder = 50;

	int m_horizontalTiles = 0;
	int m_verticalTiles = 0;

	TileNode* m_tile1 = nullptr;
	TileNode* m_tile2 = nullptr;

	bool IsInsideRect(int x, int y, SDL_Rect& rect);
	vec2 NodeToPixel(TileNode* node) const;
	void StartSwapAnimation(TileNode* tile1, TileNode* tile2, AnimationState state) const;
	void SwapTiles(TileNode* tile1, TileNode* tile2, AnimationState state);
	void SelectTile(bool isKeyDownEvent);
	bool CheckMatchAt(int x, int y, JewelColor color);
	int MatchAt(int x, int y, JewelColor color);
	void CollapseColumns();
	void RespawnTiles();
};
