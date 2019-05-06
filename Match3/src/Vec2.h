#pragma once
#include <iostream>

struct vec2 {
	float x, y;
	vec2();
	vec2(float xa, float ya);
	vec2(int xa, int ya);
	~vec2();

	friend std::ostream& operator<<(std::ostream& os, const vec2& vector);
	friend std::istream &operator>>(std::istream& is, vec2& vector);

	bool operator==(vec2& other) const;
	bool operator!=(vec2& other) const;
	vec2& operator=(const vec2& other) = default;

	vec2& operator+=(const vec2& other);
	vec2 operator+(const vec2& other) const;
	vec2& operator-=(const vec2& other);
	vec2 operator-(const vec2& other) const;

	vec2& operator*=(const float& scalar);
	vec2 operator*(const float& scalar) const;
	friend vec2 operator*(const float& scalar, const vec2& vector);
	vec2& operator/=(const float& scalar);
	vec2 operator/(const float& scalar) const;

	float Quadrance() const;
	float Magnitude() const;
	vec2& Normalize();

	float Dot(const vec2& other) const;

};