#pragma once

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

constexpr float M_GAMMA = 2.2f;
constexpr float M_PI = 3.14159265358979323846f;
constexpr float kInfinity = std::numeric_limits<float>::max();

template<typename T>
T lerp(const T& tMin, const T& tMax, const T& t)
{
	return std::max<T>(tMin, std::min<T>(tMax, t));
}