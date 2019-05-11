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

	vec2 pos{0, 0};
	JewelColor color{Red};
	bool isMatched{false};
};
