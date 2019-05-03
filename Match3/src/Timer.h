#pragma once
#include "pch.h"
#include <chrono>

struct Timer {

	static void Start();
	static float delta_ms;
	static float delta_s;

	static void Tick();
private:
	static std::chrono::time_point<std::chrono::steady_clock> m_start, m_end;
	static std::chrono::duration<float> m_deltaTime;

};