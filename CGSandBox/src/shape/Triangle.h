#pragma once

#include "Object.h"

class Triangle : public Object
{
public:
	Triangle();
	Triangle(Vec3f* vertex, Vec3f* normal, Vec2f* texCoord);
	virtual ~Triangle();

	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const override;

private:
	void getNormal(float alpha, float beta, float gamma, Vec3f& normal) const;
	void getUVCoord(float alpha, float beta, float gamma, float& u, float& v) const;

private:
	Vec3f m_Vertex[3];
	Vec3f m_Normal[3];
	Vec2f m_TexCoord[3];
};