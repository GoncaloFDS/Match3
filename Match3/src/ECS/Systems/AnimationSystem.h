#pragma once
#include "ECS/System.h"
#include "ECS/Components/Animation.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Sprite.h"

struct  AnimationNode {
	Sprite* sprite;
	Transform* transform;
	Animation* animation;

	AnimationNode(Sprite* sp, Transform* trans, Animation* anim) :
		sprite(sp),  transform(trans), animation(anim) {}
};

class AnimationSystem : public System {
public:
	void OnUpdate() override;
	void OnEvent(SDL_Event& event) override;
	void CreateNode(Entity* entity) override;

private:
	std::vector<std::unique_ptr<AnimationNode>> m_targets;

	void ComputeAnimation(AnimationNode* node, Animation*& anim) const;
	void UpdateGameState(Animation* anim);
};
