#include "pch.h"
#include "AnimationSystem.h"
#include "Game.h"


void AnimationSystem::OnUpdate() {
	for(auto& node : m_targets) {
		auto& anim = node->animation;
		if(anim->delayTime > 0)
			anim->delayTime -= Timer::delta_s;
		else if(anim->isPlaying) {
			ComputeAnimation(anim, node.get());
			if(anim->elapsedTime >= anim->duration) {
				UpdateGameState(anim);

			}
		}

	}
}

void AnimationSystem::OnEvent(SDL_Event& event) {}

void AnimationSystem::CreateNode(Entity* entity) {
	if(entity->tag == Tag::Jewel) {
		auto node = std::make_unique<AnimationNode>(&entity->GetComponent<Sprite>(),
		                                            &entity->GetComponent<Transform>(),
		                                            &entity->GetComponent<Animation>());
		m_targets.push_back(std::move(node));
	}
}

void AnimationSystem::ComputeAnimation(Animation* anim, AnimationNode* node) const {
	const auto interpolation = anim->duration == 0 ? 1 : (Timer::delta_s / anim->duration);
	node->transform->pos += (anim->endPos - anim->startPos) * interpolation;
	node->sprite->alpha += (anim->endAlpha - anim->startAlpha) * interpolation;
	node->sprite->alpha = std::clamp(node->sprite->alpha, 0.0f, 255.0f);
	anim->elapsedTime += Timer::delta_s;
}

void AnimationSystem::UpdateGameState(Animation* anim) {
	anim->isPlaying = false;
	switch(anim->state) {
	case AnimationState::Swapping:
		LOG_TRACE("State: Match");
		Game::state = State::Match;
		break;
	case AnimationState::Destroying:
		LOG_TRACE("State: Collapse");
		Game::state = State::Collapse;
		break;
	case AnimationState::Refilling:
		LOG_TRACE("State: Match from Refill");
		Game::state = State::Match;
		break;
	case AnimationState::Collapsing:
		LOG_TRACE("State: Refill");
		Game::state = State::Refill;
		break;
	default: ;
	}
	anim->state = AnimationState::None;
}
