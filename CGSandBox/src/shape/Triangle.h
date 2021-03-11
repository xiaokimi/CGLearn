#pragma once

#include "Object.h"

class Triangle : public Object
{
public:
	Triangle();
	Triangle(const std::vector<Vec3f>& position, const std::vector<Vec3f>& normal, const std::vector<Vec2f>& texCoord);
	Triangle(const std::vector<Vertex>&& vertex, const std::shared_ptr<Material>& material);
	virtual ~Triangle();

	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const override;
	
	virtual AABBox getBoundingBox() const override;
	virtual Vec3f getPointMin() const override;
	virtual Vec3f getPointMax() const override;

	virtual float getArea() const override;
	virtual bool hasEmit() const override;

	virtual void sample(HitRecord& record, float& pdf) const override;

	virtual std::vector<Object*> getPrimitiveList() const override;

private:
	void initArea();

	void getPosition(float alpha, float beta, float gamma, Vec3f& position) const;
	void getNormal(float alpha, float beta, float gamma, Vec3f& normal) const;
	void getUVCoord(float alpha, float beta, float gamma, Vec2f& texCoord) const;

private:
	float m_Area;

	std::vector<Vertex> m_Vertex;
	std::shared_ptr<Material> m_Material;
};