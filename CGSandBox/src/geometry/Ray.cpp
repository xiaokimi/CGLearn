#include "Ray.h"

Ray::Ray()
: m_Origin(0.0f)
, m_Direction(0.0f, 0.0f, -1.0f)
, m_TBorn(0.0f)
, m_TMin(0.0f)
, m_TMax(kInfinity)
{

}

Ray::Ray(const Vec3f& origin, const Vec3f& direction, float tBorn /*= 0.0f*/, float tMin /*= 0.0f*/, float tMax /*= kInfinity*/)
: m_Origin(origin)
, m_TBorn(tBorn)
, m_TMin(tMin)
, m_TMax(tMax)
{
	m_Direction = normalize(direction);
}

Ray::~Ray()
{

}

bool Ray::hit(float tMin, float tMax) const
{
	if (tMin > m_TMax || tMax < m_TMin)
	{
		return false;
	}

	return true;
}

Vec3f Ray::getOrigin() const
{
	return m_Origin;
}

Vec3f Ray::getDirection() const
{
	return m_Direction;
}

Vec3f Ray::getPosition(float t) const
{
	return m_Origin + t * m_Direction;
}

float Ray::getTimeBorn() const
{
	return m_TBorn;
}

float Ray::getTimeMin() const
{
	return m_TMin;
}

float Ray::getTimeMax() const
{
	return m_TMax;
}
