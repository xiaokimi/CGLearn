#pragma once

#include "Ray.h"

class AABBox
{
public:
	AABBox();
	AABBox(const Vec3f& pMin, const Vec3f& pMax);
	AABBox(const AABBox& box);
	virtual ~AABBox();

	AABBox& operator=(const AABBox& box);

	const Vec3f& operator[](int i) const;

	static AABBox getUnion(const AABBox& box0, const AABBox& box1);
	static AABBox getUnion(const AABBox& box, const Vec3f& p);

	Vec3f getPointMin() const;
	Vec3f getPointMax() const;
	Vec3f getCentroid() const;

	Vec3f getOffset(const Vec3f& p) const;
	float getSurfaceArea() const;

	bool hit(const Ray& ray, float tMin, float tMax) const;

private:
	Vec3f m_PointMin;
	Vec3f m_PointMax;
};

AABBox getSurroundingBox(const AABBox& box0, const AABBox& box1);