#pragma once
#include "ECS/Component.h"

struct Size : public Component {
	Size(int wh) : w(wh), h(wh), fullscreen(false) {}

	Size(int width, int height) : w(width), h(height), fullscreen(false) {}

	Size(bool bFullscreen) : fullscreen(bFullscreen) {}

	void Set(int width, int height) {
		w = width;
		h = height;
	}

	int w;
	int h;
	bool fullscreen;
};