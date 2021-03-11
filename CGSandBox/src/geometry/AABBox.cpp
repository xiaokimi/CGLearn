#include "AABBox.h"

AABBox::AABBox()
: m_PointMin(kInfinity)
, m_PointMax(-kInfinity)
{

}

AABBox::AABBox(const Vec3f& pMin, const Vec3f& pMax)
: m_PointMin(pMin)
{
	float delta = 1e-3;
	m_PointMax[0] = (pMax[0] - pMin[0] > delta) ? pMax[0] : pMin[0] + delta;
	m_PointMax[1] = (pMax[1] - pMin[1] > delta) ? pMax[1] : pMin[1] + delta;
	m_PointMax[2] = (pMax[2] - pMin[2] > delta) ? pMax[2] : pMin[2] + delta;
}


AABBox::AABBox(const AABBox& box)
: m_PointMin(box.m_PointMax)
, m_PointMax(box.m_PointMax)
{

}

AABBox::~AABBox()
{

}


AABBox& AABBox::operator=(const AABBox& box)
{
	m_PointMin = box.m_PointMin;
	m_PointMax = box.m_PointMax;

	return *this;
}

const Vec3f& AABBox::operator[](int i) const
{
	return (&m_PointMin)[i];
}


AABBox AABBox::getUnion(const AABBox& box0, const AABBox& box1)
{
	Vec3f pMin = Vec3f::min(box0.m_PointMin, box1.m_PointMin);
	Vec3f pMax = Vec3f::max(box0.m_PointMax, box1.m_PointMax);

	return AABBox(pMin, pMax);
}


AABBox AABBox::getUnion(const AABBox& box, const Vec3f& p)
{
	Vec3f pMin = Vec3f::min(box.m_PointMin, p);
	Vec3f pMax = Vec3f::max(box.m_PointMax, p);

	return AABBox(pMin, pMax);
}

Vec3f AABBox::getPointMin() const
{
	return m_PointMin;
}

Vec3f AABBox::getPointMax() const
{
	return m_PointMax;
}

Vec3f AABBox::getCentroid() const
{
	return (m_PointMin + m_PointMax) * 0.5f;
}


Vec3f AABBox::getOffset(const Vec3f& p) const
{
	float x = (p[0] - m_PointMin[0]) / (m_PointMax[0] - m_PointMin[0]);
	float y = (p[1] - m_PointMin[1]) / (m_PointMax[1] - m_PointMin[1]);
	float z = (p[2] - m_PointMin[2]) / (m_PointMax[2] - m_PointMin[2]);

	return Vec3f(x, y, z);
}


float AABBox::getSurfaceArea() const
{
	float x = m_PointMax[0] - m_PointMin[0];
	float y = m_PointMax[1] - m_PointMin[1];
	float z = m_PointMax[2] - m_PointMin[2];

	return 2 * (x * y + x * z + y * z);
}

bool AABBox::hit(const Ray& ray, float tMin, float tMax) const
{
	if (!ray.hit(tMin, tMax))
	{
		return false;
	}

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
