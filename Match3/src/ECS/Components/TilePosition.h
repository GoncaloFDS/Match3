#pragma once
#include "ECS/Component.h"

struct TilePosition : public Component {
	TilePosition(int xPos = 0, int yPos = 0) : x(xPos), y(yPos) {}

	int x, y;
};
