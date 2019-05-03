#pragma once
#include "ECS/Component.h"

struct Position : public Component {
	Position(int xy) : x(xy), y(xy) {}

	Position(int xPos = 0, int yPos = 0) : Component(), x(xPos), y(yPos) {}

	void Set(int xPos, int yPos) {
		x = xPos;
		y = yPos;
	}

	void SetX(int xPos) { x = xPos; }
	void SetY(int yPos) { y = yPos; }

	int x;
	int y;

};
