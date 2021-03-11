#include "Triangle.h"
#include "../material/Material.h"

Triangle::Triangle()
{
	Vertex v0;
	v0.position = Vec3f(0.0f);
	v0.normal   = Vec3f(0.0f, 0.0f, -1.0f);
	v0.texCoord = Vec2f(0.0f);
	m_Vertex.push_back(v0);

	Vertex v1;
	v1.position = Vec3f(1.0f, 0.0f, 0.0f);
	v1.normal   = Vec3f(0.0f, 0.0f, -1.0f);
	v1.texCoord = Vec2f(1.0f, 0.0f);
	m_Vertex.push_back(v1);

	Vertex v2;
	v2.position = Vec3f(0.0f, 1.0f, 0.0f);
	v2.normal   = Vec3f(0.0f, 0.0f, -1.0f);
	v2.texCoord = Vec2f(0.0f, 1.0f);
	m_Vertex.push_back(v2);

	initArea();
}

Triangle::Triangle(const std::vector<Vec3f>& position, const std::vector<Vec3f>& normal, const std::vector<Vec2f>& texCoord)
{
	for (int i = 0; i < 3; i++)
	{
		Vertex v;
		v.position = position[i];
		v.normal   = normal[i];
		v.texCoord = texCoord[i];

		m_Vertex.push_back(v);
	}

	initArea();
}

Triangle::Triangle(const std::vector<Vertex>&& vertex, const std::shared_ptr<Material>& material)
: m_Vertex(std::move(vertex))
, m_Material(material)
{
	initArea();
}

Triangle::~Triangle()
{

}

bool Triangle::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
	if (!ray.hit(tMin, tMax))
	{
		return false;
	}

	tMin = std::fmaxf(tMin, ray.getTimeMin());
	tMax = std::fminf(tMax, ray.getTimeMax());

	Vec3f E1 = m_Vertex[1].position - m_Vertex[0].position;
	Vec3f E2 = m_Vertex[2].position - m_Vertex[0].position;
	Vec3f S = ray.getOrigin() - m_Vertex[0].position;
	Vec3f S1 = cross(ray.getDirection(), E2);
	Vec3f S2 = cross(S, E1);
	float coeff = 1.0f / dot(S1, E1);

	float t = coeff * dot(S2, E2);
	float beta = coeff * dot(S1, S);
	float gamma = coeff * dot(S2, ray.getDirection());
	float alpha = 1.0f - beta - gamma;

	if (t > tMin && t < tMax && alpha > 0.0f && alpha < 1.0f && beta > 0.0f && beta < 1.0f && gamma > 0.0f && gamma < 1.0f)
	{
		Vec3f p = ray.getPosition(t);

		record.t = t;
		record.vertex.position = p;
		record.material = m_Material;

		getNormal(alpha, beta, gamma, record.vertex.normal);
		getUVCoord(alpha, beta, gamma, record.vertex.texCoord);

		return true;
	}

	return false;
}

AABBox Triangle::getBoundingBox() const
{
	return AABBox(getPointMin(), getPointMax());
}

Vec3f Triangle::getPointMin() const
{
	float xMin = std::fminf(m_Vertex[0].position[0], std::fminf(m_Vertex[1].position[0], m_Vertex[2].position[0]));
	float yMin = std::fminf(m_Vertex[0].position[1], std::fminf(m_Vertex[1].position[1], m_Vertex[2].position[1]));
	float zMin = std::fminf(m_Vertex[0].position[2], std::fminf(m_Vertex[1].position[2], m_Vertex[2].position[2]));

	return Vec3f(xMin, yMin, zMin);
}

Vec3f Triangle::getPointMax() const
{
	float xMax = std::fmaxf(m_Vertex[0].position[0], std::fmaxf(m_Vertex[1].position[0], m_Vertex[2].position[0]));
	float yMax = std::fmaxf(m_Vertex[0].position[1], std::fmaxf(m_Vertex[1].position[1], m_Vertex[2].position[1]));
	float zMax = std::fmaxf(m_Vertex[0].position[2], std::fmaxf(m_Vertex[1].position[2], m_Vertex[2].position[2]));

	return Vec3f(xMax, yMax, zMax);
}

float Triangle::getArea() const
{
	return m_Area;
}

bool Triangle::hasEmit() const
{
	if (m_Material)
	{
		return m_Material->hasEmission();
	}

	return false;
}

void Triangle::sample(HitRecord& record, float& pdf) const
{
	float alpha = 0.0f;
	float beta = 0.0f;

	do 
	{
		alpha = dis(gen);
		beta = dis(gen);
	} while (alpha + beta < 1.0f);

	float gamma = 1.0f - alpha - beta;

	record.material = m_Material;
	getPosition(alpha, beta, gamma, record.vertex.position);
	getNormal(alpha, beta, gamma, record.vertex.normal);

	pdf = 1.0f / m_Area;
}

std::vector<Object*> Triangle::getPrimitiveList() const
{
	std::vector<Object*> objectList;
	objectList.push_back((Object*)(this));

	return objectList;
}

void Triangle::initArea()
{
	Vec3f E1 = m_Vertex[1].position - m_Vertex[0].position;
	Vec3f E2 = m_Vertex[2].position - m_Vertex[0].position;

	m_Area = 0.5f * length(cross(E1, E2));
}

void Triangle::getPosition(float alpha, float beta, float gamma, Vec3f& position) const
{
	position = alpha * m_Vertex[0].position + beta * m_Vertex[1].position + gamma * m_Vertex[2].position;
}

void Triangle::getNormal(float alpha, float beta, float gamma, Vec3f& normal) const
{
	normal = alpha * m_Vertex[0].normal + beta * m_Vertex[1].normal + gamma * m_Vertex[2].normal;
}

void Triangle::getUVCoord(float alpha, float beta, float gamma, Vec2f& texCoord) const
{
	texCoord[0] = alpha * m_Vertex[0].texCoord[0] + beta * m_Vertex[1].texCoord[0] + gamma * m_Vertex[2].texCoord[0];
	texCoord[1] = alpha * m_Vertex[0].texCoord[1] + beta * m_Vertex[1].texCoord[1] + gamma * m_Vertex[2].texCoord[1];
}
