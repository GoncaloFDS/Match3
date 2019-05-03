#include "pch.h"
#include  "Timer.h"

float Timer::delta_ms = 0;
float Timer::delta_s = 0;
std::chrono::time_point<std::chrono::steady_clock> Timer::m_start, Timer::m_end;
std::chrono::duration<float> Timer::m_deltaTime;

void Timer::Start() {
	m_start = std::chrono::high_resolution_clock::now();
	Tick();
}

void Timer::Tick() {
	m_end = std::chrono::high_resolution_clock::now();
	m_deltaTime = m_end - m_start;
	m_start = m_end;
	delta_s = m_deltaTime.count();
	delta_ms = delta_s * 1000.0f;
}
