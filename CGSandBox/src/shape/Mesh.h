#pragma once

#include "global.h"
#include "Triangle.h"

class Mesh : public Object
{
public:
	Mesh();
	Mesh(const std::vector<Vertex>&& vertex, const std::shared_ptr<Material>& material);
	Mesh(const std::string& filePath, const std::shared_ptr<Material>& material);
	virtual ~Mesh();

	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const override;
	
	virtual AABBox getBoundingBox() const override;
	virtual Vec3f getPointMin() const override;
	virtual Vec3f getPointMax() const override;

	virtual float getArea() const override;
	virtual bool hasEmit() const override;

	virtual void sample(HitRecord& record, float& pdf) const override;

	virtual std::vector<Object*> getPrimitiveList() const override;

private:
	void initTriangleList();

private:
	float m_Area;

	std::vector<Vertex> m_Vertex;
	std::shared_ptr<Material> m_Material;

	std::vector<Triangle*> m_TriangleList;
};