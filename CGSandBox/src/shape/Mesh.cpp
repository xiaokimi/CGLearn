#include "Mesh.h"
#include "../utils/FileUtils.h"
#include "../material/Material.h"

Mesh::Mesh()
: m_Area(0.0f)
{

}

Mesh::Mesh(const std::vector<Vertex>&& vertex, const std::shared_ptr<Material>& material)
: m_Vertex(std::move(vertex))
, m_Material(material)
{
	initTriangleList();
}


Mesh::Mesh(const std::string& filePath, const std::shared_ptr<Material>& material)
: m_Material(material)
{
	if (FileUtils::loadOBJFile(filePath, m_Vertex))
	{
		initTriangleList();
	}
}

Mesh::~Mesh()
{
	for (auto triangle : m_TriangleList)
	{
		delete triangle;
	}
}

bool Mesh::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
	if (!ray.hit(tMin, tMax))
	{
		return false;
	}

	tMin = std::fmaxf(tMin, ray.getTimeMin());
	tMax = std::fminf(tMax, ray.getTimeMax());

	bool hitAnything = false;
	HitRecord tmpRecord;

	for (auto triangle : m_TriangleList)
	{
		if (triangle->hit(ray, tMin, tMax, tmpRecord))
		{
			hitAnything = true;

			tMax = tmpRecord.t;
			record = tmpRecord;
		}
	}

	return hitAnything;
}

AABBox Mesh::getBoundingBox() const
{
	return AABBox(getPointMin(), getPointMax());
}

Vec3f Mesh::getPointMin() const
{
	Vec3f pMin = Vec3f(kInfinity);

	for (const auto& vertex : m_Vertex)
	{
		pMin = Vec3f::min(pMin, vertex.position);
	}

	return pMin;
}

Vec3f Mesh::getPointMax() const
{
	Vec3f pMax = Vec3f(-kInfinity);

	for (const auto& vertex : m_Vertex)
	{
		pMax = Vec3f::max(pMax, vertex.position);
	}

	return pMax;
}

float Mesh::getArea() const
{
	return m_Area;
}

bool Mesh::hasEmit() const
{
	if (m_Material)
	{
		return m_Material->hasEmission();
	}

	return false;
}

void Mesh::sample(HitRecord& record, float& pdf) const
{
	int nSize = m_TriangleList.size();
	if (nSize == 0)
	{
		return;
	}
	else if (nSize == 1)
	{
		m_TriangleList[0]->sample(record, pdf);
	}
	else
	{
		int idx = dis(gen) * nSize;
		idx = lerp(0, nSize - 1, idx);
		m_TriangleList[idx]->sample(record, pdf);
	}
}

std::vector<Object*> Mesh::getPrimitiveList() const
{
	std::vector<Object*> objectList;
	for (Object* object : m_TriangleList)
	{
		objectList.push_back(object);
	}

	return objectList;
}

void Mesh::initTriangleList()
{
	for (int i = 0; i < m_Vertex.size(); i += 3)
	{
		std::vector<Vertex> tmpVertex;
		tmpVertex.push_back(m_Vertex[i]);
		tmpVertex.push_back(m_Vertex[i + 1]);
		tmpVertex.push_back(m_Vertex[i + 2]);

		m_TriangleList.push_back(new Triangle(std::move(tmpVertex), m_Material));
	}

	m_Area = 0.0f;
	for (const auto& triangle : m_TriangleList)
	{
		m_Area += triangle->getArea();
	}
}
