#include "pch.h"
#include "Input.h"
#include <SDL.h>

std::unique_ptr<Input> Input::m_instance = std::make_unique<Input>(); 

inline bool Input::IsKeyPressedImpl(int keyCode) {
	// TODO: Implement this method
	return false;
}

inline bool Input::IsMouseButtonPressedImpl(int button) {
	return SDL_GetMouseState(nullptr, nullptr) & button;
}

inline std::pair<float, float> Input::GetMousePosImpl() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	return {static_cast<float>(x), static_cast<float>(y)};
}

inline float Input::GetMouseXImpl() {
	auto [x, y] = GetMousePosImpl();
	return x;
}

inline float Input::GetMouseYImpl() {
	auto [x ,y] = GetMousePosImpl();
	return y;
}
