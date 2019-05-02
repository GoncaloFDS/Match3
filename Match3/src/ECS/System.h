#pragma once

class System {
public:
	virtual ~System() = default;
	virtual void OnUpdate() = 0;
	virtual void CreateNode(Entity* entity) = 0;

};
