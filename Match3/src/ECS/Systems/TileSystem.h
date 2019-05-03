#pragma once
#include "pch.h"
#include "ECS/System.h"

struct Position;
class Sprite;
struct Size;
class Entity;
struct SDL_Rect;

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

	JewelColor color;

	TileNode(Sprite* sp, Position* pos, Size* sz) :
		sprite(sp), position(pos), size(sz), color(Red) {}
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

	bool IsInsideRect(int x, int y, SDL_Rect& rect);
	void SelectTile();

};
