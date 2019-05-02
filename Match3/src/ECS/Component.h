#pragma once
#include "pch.h"

class Entity;

using ComponentId = unsigned int;

class Component {
public:
	Entity* entity;

};

inline ComponentId GetComponentTypeId() {
	static ComponentId lastId = 0;
	return lastId++;
}

template<typename T>
inline ComponentId GetComponentTypeId() noexcept {
	static ComponentId typeId = GetComponentTypeId();
	return typeId;
}
