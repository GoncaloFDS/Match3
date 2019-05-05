#pragma once
#include "ECS/System.h"
#include <SDL.h>

class Entity;
class Sprite;
struct Transform;

struct RenderNode {
	Sprite* sprite;
	Transform* transform;

	RenderNode(Sprite* sp, Transform* trans) : sprite(sp), transform(trans) {}
};

class RenderSystem : public System {

public:

	void OnStart(SDL_Window* window, const std::string& title, int width, int height, bool fullscreen);
	void OnUpdate() override;
	void OnEvent(SDL_Event& event) override;

	void CreateNode(Entity* entity) override;

private:
	std::vector<std::unique_ptr<RenderNode>> m_targets;

};
