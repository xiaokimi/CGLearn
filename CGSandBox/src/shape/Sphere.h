#pragma once

#include "Object.h"

class Sphere : public Object
{
public:
	Sphere();
	Sphere(const Vec3f& center, float radius);
	virtual ~Sphere();

	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const override;
	virtual AABBox getBoundingBox() const override;
	virtual Vec3f getPointMin() const override;
	virtual Vec3f getPointMax() const override;

protected:
	void getUVCoord(const Vec3f& point, float& u, float& v) const;

private:
	Vec3f m_Center;
	float m_Radius;
};