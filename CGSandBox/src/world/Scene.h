#pragma once

#include "../geometry/BVH.h"

class Scene
{
public:
	Scene();
	Scene(int width, int height);
	virtual ~Scene();

	void setObjectList(std::vector<Object*>&& objectList);

	int getWidth() const;
	int getHeight() const;

	bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const;

	void sampleLight(HitRecord& record, float& pdf) const;

private:
	void clear();

private:
	int m_Width;
	int m_Height;

	std::vector<Object*> m_ObjectList;

	BVH* m_BVHRoot;
};