#pragma once
#include "ECS/System.h"
#include <SDL.h>

class Entity;
struct Size;
class Sprite;
struct Position;

struct RenderNode {
	Sprite* sprite;
	Position* position;
	Size* size;

	RenderNode(Sprite* sp, Position* pos, Size* sz) : sprite(sp), position(pos), size(sz) {}
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
