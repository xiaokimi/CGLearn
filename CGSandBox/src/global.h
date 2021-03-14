#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <memory>
#include <limits>
#include <random>
#include <thread>
#include <mutex>
#include <functional>
#include <algorithm>

#include "geometry/Vector.h"

constexpr float M_GAMMA = 2.2f;
constexpr float M_PI = 3.14159265358979323846f;
constexpr float kInfinity = std::numeric_limits<float>::max();

static std::random_device dev;
static std::mt19937 gen(dev());
static std::uniform_real_distribution<float> dis(0.0f, 1.0f);

template<typename T>
T clamp(const T& tMin, const T& tMax, const T& t)
{
	return std::max<T>(tMin, std::min<T>(tMax, t));
}

static Vec3f getRandomInUnitSphere()
{
	Vec3f point;
	do
	{
		point = 2.0f * Vec3f(dis(gen), dis(gen), dis(gen)) - Vec3f(1.0f);
	} while (lengthSquared(point) >= 1.0f);

	return point;
}