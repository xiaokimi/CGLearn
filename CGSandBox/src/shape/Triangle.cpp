#include "cgpch.h"
#include "Triangle.h"

Triangle::Triangle()
{
	m_Vertex[0] = Vec3f(0);
	m_Vertex[1] = Vec3f(1, 0, 0);
	m_Vertex[2] = Vec3f(0, 1, 0);

	m_Normal[0] = Vec3f(0, 0, -1);
	m_Normal[1] = Vec3f(0, 0, -1);
	m_Normal[2] = Vec3f(0, 0, -1);

	m_TexCoord[0] = Vec2f(0);
	m_TexCoord[0] = Vec2f(1, 0);
	m_TexCoord[0] = Vec2f(0, 1);
}

Triangle::Triangle(Vec3f* vertex, Vec3f* normal, Vec2f* texCoord)
{
	for (int i = 0; i < 3; i++)
	{
		m_Vertex[i] = vertex[i];
		m_Normal[i] = normal[i];
		m_TexCoord[i] = texCoord[i];
	}
}

Triangle::~Triangle()
{

}

bool Triangle::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
	tMin = std::fmaxf(tMin, ray.getTimeMin());
	tMax = std::fminf(tMax, ray.getTimeBorn());

	if (tMin > tMax)
	{
		return false;
	}

	Vec3f E1 = m_Vertex[1] - m_Vertex[0];
	Vec3f E2 = m_Vertex[2] - m_Vertex[0];
	Vec3f S = ray.getOrigin() - m_Vertex[0];
	Vec3f S1 = cross(ray.getDirection(), E2);
	Vec3f S2 = cross(S, E1);
	float coeff = 1.0f / dot(S1, E1);

	float t = coeff * dot(S2, E2);
	float beta = coeff * dot(S1, S);
	float gamma = coeff * dot(S2, ray.getDirection());
	float alpha = 1.0f - beta - gamma;

	if (t > tMin && t < tMax && alpha > 0.0f && alpha < 1.0f && beta > 0.0f && beta < 1.0f && gamma > 0.0f && gamma < 1.0f)
	{
		record.t = t;
		record.p = ray.getPosition(t);

		getNormal(alpha, beta, gamma, record.normal);
		getUVCoord(alpha, beta, gamma, record.u, record.v);

		return true;
	}

	return false;
}

AABBox Triangle::getBoundingBox() const
{
	float xMin = std::fminf(m_Vertex[0][0], std::fminf(m_Vertex[1][0], m_Vertex[2][0]));
	float yMin = std::fminf(m_Vertex[0][1], std::fminf(m_Vertex[1][1], m_Vertex[2][1]));
	float zMin = std::fminf(m_Vertex[0][2], std::fminf(m_Vertex[1][2], m_Vertex[2][2]));

	float xMax = std::fmaxf(m_Vertex[0][0], std::fmaxf(m_Vertex[1][0], m_Vertex[2][0]));
	float yMax = std::fmaxf(m_Vertex[0][1], std::fmaxf(m_Vertex[1][1], m_Vertex[2][1]));
	float zMax = std::fmaxf(m_Vertex[0][2], std::fmaxf(m_Vertex[1][2], m_Vertex[2][2]));

	float delta = 1e-6;
	xMax = (xMax - xMin > delta) ? xMax : xMin + delta;
	yMax = (yMax - yMin > delta) ? yMax : yMin + delta;
	zMax = (zMax - zMin > delta) ? zMax : zMin + delta;

	return AABBox(Vec3f(xMin, yMin, zMin), Vec3f(xMax, yMax, zMax));
}

void Triangle::getNormal(float alpha, float beta, float gamma, Vec3f& normal) const
{
	normal = alpha * m_Normal[0] + beta * m_Normal[1] + gamma * m_Normal[2];
}

void Triangle::getUVCoord(float alpha, float beta, float gamma, float& u, float& v) const
{
	u = alpha * m_TexCoord[0][0] + beta * m_TexCoord[1][0] + gamma * m_TexCoord[2][0];
	v = alpha * m_TexCoord[0][1] + beta * m_TexCoord[1][1] + gamma * m_TexCoord[2][1];
}
