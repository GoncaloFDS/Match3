#pragma once
#include "Entity.h"
#include <SDL.h>

class System {
public:
	virtual ~System() = default;
	virtual void OnStart() {}
	virtual void OnUpdate() = 0;
	virtual void OnEvent(SDL_Event& event) = 0;
	virtual void CreateNode(Entity* entity) = 0;

};
