#pragma once

#include "global.h"
#include "../geometry/AABBox.h"

class Material;

struct Vertex
{
	Vec3f position;
	Vec3f normal;
	Vec2f texCoord;
};

struct HitRecord
{
	float t;
	Vertex vertex;
	std::shared_ptr<Material> material;
};

class Object
{
public:
	Object() {}
	virtual ~Object() {}

	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const = 0;

	virtual AABBox getBoundingBox() const = 0;
	virtual Vec3f getPointMin() const = 0;
	virtual Vec3f getPointMax() const = 0;

	virtual float getArea() const = 0;
	virtual bool hasEmit() const = 0;

	virtual void sample(HitRecord& record, float& pdf) const = 0;

	virtual std::vector<Object*> getPrimitiveList() const = 0;
};