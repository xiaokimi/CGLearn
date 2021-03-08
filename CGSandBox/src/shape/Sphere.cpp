#include "Sphere.h"

Sphere::Sphere()
: m_Center(0)
, m_Radius(1)
{

}

Sphere::Sphere(const Vec3f& center, float radius)
: m_Center(center)
, m_Radius(radius)
{

}

Sphere::~Sphere()
{

}

bool Sphere::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
	tMin = std::fmaxf(tMin, ray.getTimeMin());
	tMax = std::fminf(tMax, ray.getTimeMax());

	if (tMin > tMax)
	{
		return false;
	}

	Vec3f oc = ray.getOrigin() - m_Center;
	float a = dot(ray.getDirection(), ray.getDirection());
	float halfb = dot(ray.getDirection(), oc);
	float c = dot(oc, oc) - m_Radius * m_Radius;
	float discriminant = halfb * halfb - a * c;
	if (discriminant > 0)
	{
		float t = (-halfb - std::sqrtf(discriminant)) / a;
		if (t > tMin && t < tMax)
		{
			record.t = t;
			record.p = ray.getPosition(t);
			record.normal = (record.p - m_Center) / m_Radius;

			getUVCoord(record.p, record.uv);
			return true;
		}

		t = (-halfb + std::sqrtf(discriminant)) / a;
		if (t > tMin && t < tMax)
		{
			record.t = t;
			record.p = ray.getPosition(t);
			record.normal = (record.p - m_Center) / m_Radius;

			getUVCoord(record.p, record.uv);
			return true;
		}
	}

	return false;
}

AABBox Sphere::getBoundingBox() const
{
	return AABBox(getPointMin(), getPointMax());
}

Vec3f Sphere::getPointMin() const
{
	return m_Center - Vec3f(m_Radius);
}

Vec3f Sphere::getPointMax() const
{
	return m_Center + Vec3f(m_Radius);
}

void Sphere::getUVCoord(const Vec3f& point, Vec2f& uv) const
{
	Vec3f p = (point - m_Center) / m_Radius;
	float phi = std::atan2f(p[1], p[0]);

	uv[0] = phi / (2 * M_PI) + 0.5;
	uv[1] = p[2] * 0.5 + 0.5;
}
