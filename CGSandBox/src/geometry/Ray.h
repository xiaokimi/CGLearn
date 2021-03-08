#pragma once

#include "global.h"
#include "Vector.h"

class Ray
{
public:
	Ray();
	Ray(const Vec3f& origin, const Vec3f& direction, float tBorn = 0.0f, float tMin = 0.0f, float tMax = kInfinity);
	virtual ~Ray();

	bool hit(float tMin, float tMax) const;

	Vec3f getOrigin() const;
	Vec3f getDirection() const;
	Vec3f getPosition(float t) const;

	float getTimeBorn() const;
	float getTimeMin() const;
	float getTimeMax() const;

private:
	Vec3f m_Origin;
	Vec3f m_Direction;

	float m_TBorn;

	float m_TMin;
	float m_TMax;
};