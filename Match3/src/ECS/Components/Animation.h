#pragma once
#include "ECS/Component.h"

struct Animation : public Component {
	vec2 start;
	vec2 end;
	float elapsedTime{0};
	float duration{0};
	bool isPlaying = false;
};
