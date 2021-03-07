#pragma once

#include "Triangle.h"

class Mesh : public Object
{
public:
	Mesh();
	Mesh(Vec3f* vertex, int* indices, Vec3f* normal, Vec2f* texCoord, int nCount);
	virtual ~Mesh();

	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const override;
	virtual AABBox getBoundingBox() const override;
	virtual Vec3f getPointMin() const override;
	virtual Vec3f getPointMax() const override;

private:
	Vec3f* m_Vertex;

	int* m_Indices;
	Vec3f* m_Normal;
	Vec2f* m_TexCoord;

	Triangle* m_TriangleList;
	int m_TriangleCount;
};