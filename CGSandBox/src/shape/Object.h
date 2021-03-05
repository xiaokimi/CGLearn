#pragma once

#include "../geometry/Ray.h"

struct HitRecord
{
	float t;

	Vec3f p;
	Vec3f normal;

	float u;
	float v;
};

class Object
{
public:
	Object() {}
	virtual ~Object() {}

	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const = 0;
};