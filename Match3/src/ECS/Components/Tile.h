#pragma once
#include "ECS/Component.h"

enum JewelColor {
	Red = 0,
	Black,
	White,
	Pink,
	Blue
};

struct Tile : public Component {

	vec2 pos;
	JewelColor color;
	bool isMatched;
};
