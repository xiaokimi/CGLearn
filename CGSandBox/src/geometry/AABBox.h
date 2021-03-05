#pragma once

#include "Vector.h"
#include "Ray.h"

class AABBox
{
public:
	AABBox();
	AABBox(const Vec3f& pMin, const Vec3f& pMax);
	virtual ~AABBox();

	const Vec3f& operator[](int i) const;

	Vec3f getPointMin() const;
	Vec3f getPointMax() const;

	bool hit(const Ray& ray, float tMin, float tMax) const;

private:
	Vec3f m_PointMin;
	Vec3f m_PointMax;
};

AABBox getSurroundingBox(const AABBox& box0, const AABBox& box1);