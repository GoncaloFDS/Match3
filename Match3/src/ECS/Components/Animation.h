#pragma once
#include "ECS/Component.h"

enum class AnimationState {
	None = 0,
	Swapping,
	Exploding,
	Collapsing
};

struct Animation : public Component {
	vec2 start;
	vec2 end;
	float delayTime{0};
	float elapsedTime{0};
	float duration{0};
	bool isPlaying{false};
	AnimationState state{AnimationState::None};
};
