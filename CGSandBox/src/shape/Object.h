#pragma once

#include "../geometry/Ray.h"
#include "../geometry/AABBox.h"

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
	virtual AABBox getBoundingBox() const = 0;
	virtual Vec3f getPointMin() const = 0;
	virtual Vec3f getPointMax() const = 0;
};