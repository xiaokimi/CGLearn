#include "Sphere.h"
#include "../material/Material.h"

Sphere::Sphere()
: m_Center(0)
, m_Radius(1)
, m_Area(4 * M_PI)
{

}

Sphere::Sphere(const Vec3f& center, float radius, std::shared_ptr<Material>& material)
: m_Center(center)
, m_Radius(radius)
, m_Area(4 * M_PI * radius)
, m_Material(material)
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
			Vec3f p = ray.getPosition(t);

			record.t = t;
			record.vertex.position = p;
			record.vertex.normal = (p - m_Center) / m_Radius;

			getUVCoord(p, record.vertex.texCoord);
			return true;
		}

		t = (-halfb + std::sqrtf(discriminant)) / a;
		if (t > tMin && t < tMax)
		{
			Vec3f p = ray.getPosition(t);

			record.t = t;
			record.vertex.position = p;
			record.vertex.normal = (p - m_Center) / m_Radius;

			getUVCoord(p, record.vertex.texCoord);
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

float Sphere::getArea() const
{
	return m_Area;
}

bool Sphere::hasEmit() const
{
	if (m_Material)
	{
		return m_Material->hasEmission();
	}

	return false;
}

void Sphere::sample(HitRecord& record, float& pdf) const
{
	float theta = M_PI * dis(gen);
	float phi = 2 * M_PI * dis(gen);

	Vec3f direction = Vec3f(std::sinf(theta) * std::sinf(phi), std::cosf(theta), std::sinf(theta) * std::cosf(phi));
	
	record.vertex.position = m_Center + m_Radius * direction;
	record.vertex.normal = direction;
	record.material = m_Material;

	pdf = 1.0f / m_Area;
}

std::vector<Object*> Sphere::getPrimitiveList() const
{
	std::vector<Object*> objectList;
	objectList.push_back((Object*)(this));

	return objectList;
}

void Sphere::getUVCoord(const Vec3f& point, Vec2f& texCoord) const
{
	Vec3f p = (point - m_Center) / m_Radius;
	float phi = std::atan2f(p[1], p[0]);

	texCoord[0] = phi / (2 * M_PI) + 0.5;
	texCoord[1] = p[2] * 0.5 + 0.5;
}
