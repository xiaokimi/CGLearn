#pragma once

#include "Object.h"

class Sphere : public Object
{
public:
	Sphere();
	Sphere(const Vec3f& center, float radius, std::shared_ptr<Material>& material);
	virtual ~Sphere();

	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const override;
	
	virtual AABBox getBoundingBox() const override;
	virtual Vec3f getPointMin() const override;
	virtual Vec3f getPointMax() const override;

	virtual float getArea() const override;
	virtual bool hasEmit() const override;

	virtual void sample(HitRecord& record, float& pdf) const override;

	virtual std::vector<Object*> getPrimitiveList() const override;

protected:
	void getUVCoord(const Vec3f& point, Vec2f& texCoord) const;

private:
	Vec3f m_Center;
	float m_Radius;
	float m_Area;

	std::shared_ptr<Material> m_Material;
};