#include "Scene.h"

Scene::Scene()
: m_Width(100)
, m_Height(100)
, m_BVHRoot(nullptr)
{

}

Scene::Scene(int width, int height)
: m_Width(width)
, m_Height(height)
, m_BVHRoot(nullptr)
{

}

Scene::~Scene()
{
	clear();
}

void Scene::setObjectList(std::vector<Object*>&& objectList)
{
	clear();

	m_ObjectList = std::move(objectList);
	m_BVHRoot = new BVH(m_ObjectList, 1, SplitMethod::SAH);
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
	if (!m_BVHRoot)
	{
		return false;
	}

	return (m_BVHRoot->hit(ray, tMin, tMax, record));
}


void Scene::sampleLight(HitRecord& record, float& pdf) const
{
	float emitAreaSum = 0.0f;
	for (const auto& object : m_ObjectList)
	{
		if (object->hasEmit())
		{
			emitAreaSum += object->getArea();
		}
	}

	float p = dis(gen) * emitAreaSum;
	emitAreaSum = 0.0f;

	for (const auto& object : m_ObjectList)
	{
		if (object->hasEmit())
		{
			emitAreaSum += object->getArea();
			if (p < emitAreaSum)
			{
				object->sample(record, pdf);
				break;
			}
		}
	}
}

void Scene::clear()
{
	for (auto object : m_ObjectList)
	{
		delete object;
	}
	m_ObjectList.clear();

	delete m_BVHRoot;
	m_BVHRoot = nullptr;
}
