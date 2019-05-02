#pragma once
#include "pch.h"
#include "ECS/System.h"
#include "ECS/Components/Position.h"

struct TileNode {
	Position* position;

	TileNode(Position* p) : position(p) {}
};

class TileSystem : public System {

public:
	void Update() override {
		for(TileNode* node : m_targets) {
			node->position->SetX(node->position->x() + 1);
			if(Input::IsMouseButtonPressed(SDL_BUTTON(SDL_BUTTON_LEFT))) {
				node->position->Set(0, 0);
			}
		}
	}

	void CreateNode(Entity* entity) override {
		if(entity->ContainsComponent<Position>()) {
			auto* node = new TileNode(&entity->GetComponent<Position>());
			m_targets.push_back(node);
		}

	}

private:
	std::vector<TileNode*> m_targets;
};
