#include "cgpch.h"
#include "Scene.h"

Scene::Scene()
: m_Width(100)
, m_Height(100)
, m_ObjectList(nullptr)
, m_BVH(nullptr)
{

}

Scene::Scene(int width, int height)
: m_Width(width)
, m_Height(height)
, m_ObjectList(nullptr)
, m_BVH(nullptr)
{

}

Scene::~Scene()
{
	delete[] m_ObjectList;
	delete m_BVH;
}

void Scene::setObjectList(Object* objectList, int nCount)
{
	delete[] m_ObjectList;
	delete m_BVH;

	m_ObjectList = objectList;
	m_BVH = new BVH(objectList, nCount);
}

int Scene::getWidth() const
{
	return m_Width;
}

int Scene::getHeight() const
{
	return m_Height;
}

bool Scene::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
	if (!m_BVH)
	{
		return false;
	}

	return (m_BVH->hit(ray, tMin, tMax, record));
}
