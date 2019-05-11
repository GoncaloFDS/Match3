#pragma once
#include "ECS/Component.h"

enum class AnimationState {
	None = 0,
	Swapping,
	Destroying,
	Collapsing,
	Refilling
};

struct Animation : public Component {
	vec2 startPos;
	vec2 endPos;
	float startAlpha;
	float endAlpha;
	float delayTime{0};
	float elapsedTime{0};
	float duration{0};
	bool isPlaying{false};
	AnimationState state{AnimationState::None};
};
