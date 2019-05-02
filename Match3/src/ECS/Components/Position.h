#pragma once
#include "ECS/Component.h"

class Position : public Component {

public:
	Position(int xy) : m_x(xy), m_y(xy) {}

	Position(int x = 0, int y = 0) : m_x(x), m_y(y) {}

	int x() { return m_x; }
	int y() { return m_y; }

	void Set(int x, int y) {
		m_x = x;
		m_y = y;
	}

	void SetX(int x) { m_x = x; }
	void SetY(int y) { m_y = y; }

private:
	int m_x;
	int m_y;

};
