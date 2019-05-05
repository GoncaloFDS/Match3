#pragma once
#include "ECS/Component.h"

struct Transform : public Component {
	Transform(const vec2& position, const vec2& sz) : pos(position), size(sz) {}
	Transform(bool bFullscreen) : isFullscreen(bFullscreen) {}
	vec2 pos;
	vec2 size;

	bool isFullscreen = false;
};
