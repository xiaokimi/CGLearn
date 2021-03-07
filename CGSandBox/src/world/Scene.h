#pragma once

#include "../geometry/BVH.h"

class Scene
{
public:
	Scene();
	Scene(int width, int height);
	virtual ~Scene();

	void setObjectList(Object** objectList, int nCount);

	int getWidth() const;
	int getHeight() const;

	bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const;

private:
	int m_Width;
	int m_Height;

	Object* m_ObjectList;

	BVH* m_BVH;
};