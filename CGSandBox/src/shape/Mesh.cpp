#include "cgpch.h"
#include "Mesh.h"

Mesh::Mesh()
{
	m_Vertex = new Vec3f[3];
	m_Vertex[0] = Vec3f(0);
	m_Vertex[1] = Vec3f(1, 0, 0);
	m_Vertex[2] = Vec3f(0, 1, 0);

	m_Indices = new int[3];
	m_Indices[0] = 0;
	m_Indices[1] = 1;
	m_Indices[2] = 2;

	m_Normal = new Vec3f[3];
	m_Normal[0] = Vec3f(0, 0, -1);
	m_Normal[1] = Vec3f(0, 0, -1);
	m_Normal[2] = Vec3f(0, 0, -1);

	m_TexCoord = new Vec2f[3];
	m_TexCoord[0] = Vec2f(0);
	m_TexCoord[1] = Vec2f(1, 0);
	m_TexCoord[2] = Vec2f(0, 1);

	m_TriangleList = new Triangle[1];
	m_TriangleList[0] = Triangle(m_Vertex, m_Normal, m_TexCoord);

	m_TriangleCount = 1;
}

Mesh::Mesh(Vec3f* vertex, int* indices, Vec3f* normal, Vec2f* texCoord, int nCount)
: m_Vertex(vertex)
, m_Indices(indices)
, m_Normal(normal)
, m_TexCoord(texCoord)
, m_TriangleCount(nCount)
{
	m_TriangleList = new Triangle[nCount];
	for (int i = 0; i < nCount; i++)
	{
		Vec3f triangleVertex[3] = {
			vertex[indices[i * 3]],
			vertex[indices[i * 3 + 1]],
			vertex[indices[i * 3 + 2]] };

		m_TriangleList[i] = Triangle(triangleVertex, normal + i * 3, texCoord + i * 3);
	}
}

Mesh::~Mesh()
{
	delete[] m_Vertex;
	delete[] m_Indices;
	delete[] m_Normal;
	delete[] m_TexCoord;
	delete[] m_TriangleList;
}

bool Mesh::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
	bool hitAnything = false;

	HitRecord tmpRecord;
	for (int i = 0; i < m_TriangleCount; i++)
	{
		if (m_TriangleList[0].hit(ray, tMin, tMax, tmpRecord))
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
	float xMin = m_Vertex[0][0];
	float yMin = m_Vertex[0][1];
	float zMin = m_Vertex[0][2];

	float xMax = xMin;
	float yMax = yMin;
	float zMax = zMin;

	for (int i = 1; i < m_TriangleCount * 3; i++)
	{
		xMin = std::fminf(xMin, m_Vertex[i][0]);
		yMin = std::fminf(yMin, m_Vertex[i][1]);
		zMin = std::fminf(zMin, m_Vertex[i][2]);

		xMax = std::fmaxf(xMax, m_Vertex[i][0]);
		yMax = std::fmaxf(yMax, m_Vertex[i][0]);
		zMax = std::fmaxf(zMax, m_Vertex[i][0]);
	}

	float delta = 1e-6;
	xMax = (xMax - xMin > delta) ? xMax : xMin + delta;
	yMax = (yMax - yMin > delta) ? yMax : yMin + delta;
	zMax = (zMax - zMin > delta) ? zMax : zMin + delta;

	return AABBox(Vec3f(xMin, yMin, zMin), Vec3f(xMax, yMax, zMax));
}
