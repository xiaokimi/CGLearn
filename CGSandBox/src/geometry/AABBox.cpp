#include "cgpch.h"
#include "AABBox.h"

AABBox::AABBox()
: m_PointMin(0)
, m_PointMax(1)
{

}

AABBox::AABBox(const Vec3f& pMin, const Vec3f& pMax)
: m_PointMin(pMin)
, m_PointMax(pMax)
{

}

AABBox::~AABBox()
{

}

const Vec3f& AABBox::operator[](int i) const
{
	return (&m_PointMin)[i];
}

Vec3f AABBox::getPointMin() const
{
	return m_PointMin;
}

Vec3f AABBox::getPointMax() const
{
	return m_PointMax;
}

bool AABBox::hit(const Ray& ray, float tMin, float tMax) const
{
	Vec3f origin = ray.getOrigin();
	Vec3f direction = ray.getDirection();

	Vec3f pMin = getPointMin();
	Vec3f pMax = getPointMax();

	for (int i = 0; i < 3; i++)
	{
		float invD = 1.0f / direction[i];
		float t0 = (pMin[i] - origin[i]) * invD;
		float t1 = (pMax[i] - origin[i]) * invD;
		if (invD < 0.0f)
		{
			std::swap(t0, t1);
		}

		tMin = std::fmaxf(t0, tMin);
		tMax = std::fminf(t1, tMax);

		if (tMax <= tMin)
		{
			return false;
		}
	}

	return true;
}

AABBox getSurroundingBox(const AABBox& box0, const AABBox& box1)
{
	Vec3f pMin0 = box0.getPointMin();
	Vec3f pMin1 = box1.getPointMin();
	float xMin = std::fminf(pMin0[0], pMin1[0]);
	float yMin = std::fminf(pMin0[1], pMin1[1]);
	float zMin = std::fminf(pMin0[2], pMin1[2]);

	Vec3f pMax0 = box0.getPointMax();
	Vec3f pMax1 = box1.getPointMax();
	float xMax = std::fmaxf(pMax0[0], pMax1[0]);
	float yMax = std::fmaxf(pMax0[1], pMax1[1]);
	float zMax = std::fmaxf(pMax0[2], pMax1[2]);

	return AABBox(Vec3f(xMin, yMin, zMin), Vec3f(xMax, yMax, zMax));
}
