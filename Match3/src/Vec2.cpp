#include "pch.h"
#include "vec2.h"
#include <tuple>

vec2::vec2() = default;

vec2::vec2(const float xa, const float ya) : x(xa), y(ya) {}

vec2::vec2(const int xa, const int ya) : x(xa), y(ya) {}

vec2::~vec2() = default;

std::ostream& operator<<(std::ostream& os, const vec2& vec) {
	os << "(" << vec.x << ", " << vec.y << ")";
	return os;
}

std::istream& operator>>(std::istream& is, vec2& vector) {
	is >> vector.x;
	is >> vector.y;
	return is;
}

bool vec2::operator==(vec2& other) const {
	return x == other.x && y == other.y;
}

bool vec2::operator!=(vec2& other) const {
	return !(*this == other);
}

vec2& vec2::operator=(const vec2& other) {
	x = other.x;
	y = other.y;
	return *this;
}


vec2& vec2::operator+=(const vec2 & other) {
	x += other.x;
	y += other.y;
	return *this;
}

vec2 vec2::operator+(const vec2& other) const {
	return vec2(x + other.x, y + other.y);
}

vec2& vec2::operator-=(const vec2& other) {
	x -= other.x;
	y -= other.y;
	return *this;
}

vec2 vec2::operator-(const vec2& other) const {
	return vec2(x - other.x, y - other.y);
}

vec2& vec2::operator*=(const float& scalar) {
	x *= scalar;
	y *= scalar;
	return *this;
}

vec2 vec2::operator*(const float& scalar) const {
	return vec2(x*scalar, y*scalar);
}

vec2 operator*(const float& scalar, const vec2& vector) {
	return vec2(vector.x*scalar, vector.y*scalar);
}

vec2& vec2::operator/=(const float& scalar) {
	x /= scalar;
	y /= scalar;
	return *this;
}

vec2 vec2::operator/(const float& scalar) const {
	return vec2(x / scalar, y / scalar);
}

float vec2::Quadrance() const {
	return x*x + y*y;
}

float vec2::Magnitude() const {
	return sqrt(Quadrance());
}

vec2& vec2::Normalize() {
	//if (*this == Vec2(0, 0)) throw NormalizeZeroVetor();
	const float magnitude = this->Magnitude();
	x /= magnitude;
	y /= magnitude;
	return *this;
}

float vec2::Dot(const vec2& other) const {
	return x * other.x + y * other.y;
}
