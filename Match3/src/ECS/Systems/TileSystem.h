#pragma once
#include "pch.h"
#include "ECS/System.h"

class Sprite;
class Entity;
struct Position;
struct Size;
struct SDL_Rect;
struct TilePosition;

enum JewelColor {
	Red = 0,
	Black,
	White,
	Pink,
	Blue
};

struct TileNode {
	Sprite* sprite;
	Position* position;
	Size* size;
	TilePosition* tilePosition;

	JewelColor color;

	TileNode(Sprite* sp, Position* pos, Size* sz, TilePosition* tp) :
		sprite(sp), position(pos), size(sz), tilePosition(tp), color(Red) {}
};

class TileSystem : public System {

public:
	void OnStart(int horizontalTiles, int verticalTiles);

	void OnUpdate() override;
	void OnEvent(SDL_Event& event) override;

	void CreateNode(Entity* entity) override;

private:
	//std::vector<std::unique_ptr<TileNode>> m_targets;
	std::vector<std::vector<std::unique_ptr<TileNode>>> m_tiles;

	const int xBorder = 200;
	const int yBorder = 120;
	const int innerBorder = 50;

	int m_horizontalTiles;
	int m_verticalTiles;

	TileNode* m_tile1 = nullptr;
	TileNode* m_tile2 = nullptr;

	bool IsInsideRect(int x, int y, SDL_Rect& rect);
	void SwapTiles();
	void SelectTile(bool isKeyDownEvent);

};
