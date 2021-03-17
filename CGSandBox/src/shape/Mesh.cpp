#include "Mesh.h"
#include "../utils/FileUtils.h"
#include "../material/Material.h"
#include "LoopSubDivision.h"

Mesh::Mesh()
: m_Area(0.0f)
, m_FaceType(FaceType::TYPE_NONE)
, m_SubDivideLevel(0)
{

}

Mesh::Mesh(const std::vector<Vertex>&& vertex, const std::shared_ptr<Material>& material)
: m_Vertex(std::move(vertex))
, m_Material(material)
, m_FaceType(FaceType::TYPE_NONE)
, m_SubDivideLevel(0)
{
	initTriangleList();
}


Mesh::Mesh(const std::string& filePath, const std::shared_ptr<Material>& material)
: m_Material(material)
, m_SubDivideLevel(0)
{
	if (FileUtils::loadOBJFile(filePath, m_Vertices, m_Normals, m_TexCoords, m_Faces, m_FaceType))
	{
		updateVertex();
		initTriangleList();
	}
}

Mesh::~Mesh()
{
	clear();
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
		idx = clamp(0, nSize - 1, idx);
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


void Mesh::loopSubDivide(int level)
{
	if (m_SubDivideLevel == level)
	{
		return;
	}

	m_SubDivideLevel = level;

	if (level == 0)
	{
		// 
		updateVertex();
		initTriangleList();
		return;
	}

	// init vertex
	std::vector<SDVertex*> vertices;
	for (const auto& point : m_Vertices)
	{
		vertices.push_back(new SDVertex(point));
	}
	
	// init face
	int nVertexSize = m_Vertices.size();
	std::vector<SDFace*> faces;
	for (const auto& f : m_Faces)
	{
		SDFace* face = new SDFace();
		for (int i = 0; i < 3; i++)
		{
			int idx = f.faceVertex[i].vIndex;
			idx = (idx < 0) ? nVertexSize + idx : idx - 1;

			SDVertex* vertex = vertices[idx];
			face->vertices[i] = vertex;
			vertex->face = face;
		}

		faces.push_back(face);
	}

	// set neighbor face
	std::set<SDEdge> edges;
	for (auto& face : faces)
	{
		for (int i = 0; i < 3; i++)
		{
			int nextIdx = NEXT(i);
			SDEdge edge(face->vertices[i], face->vertices[nextIdx]);
			
			auto iter = edges.find(edge);
			if (iter == edges.end())
			{
				edge.faces[0] = face;
				edge.face0index = i;
				edges.insert(edge);
			}
			else
			{
				edge = *iter;
				edge.faces[0]->neighborFaces[edge.face0index] = face;
				face->neighborFaces[i] = edge.faces[0];
				edges.erase(edge);
			}
		}
	}
	
	// finish vertex
	for (auto& vertex : vertices)
	{
		SDFace* face = vertex->face;
		do 
		{
			face = face->getNextFace(vertex);
		} while (face && face != vertex->face);

		vertex->boundary = (face == nullptr);
	}

	// prepare next subdivision
	std::vector<SDFace*> f = faces;
	std::vector<SDVertex*> v = vertices;

	for (int i = 0; i < level; i++)
	{
		std::vector<SDFace*> newFaces;
		std::vector<SDVertex*> newVertices;

		for (auto& vertex : v)
		{
			SDVertex* child = new SDVertex();
			child->boundary = vertex->boundary;

			vertex->child = child;
			newVertices.push_back(child);
		}

		for (auto& face : f)
		{
			for (int i = 0; i < 4; i++)
			{
				SDFace* child = new SDFace();
				
				face->children[i] = child;
				newFaces.push_back(child);
			}
		}

		// update old vertex point
		for (const auto& vertex : v)
		{
			vertex->child->point = vertex->getOldAdjustPosition();
		}

		// compute new vertex point
		std::map<SDEdge, SDVertex*> edgeVerts;
		for (const auto& face : f)
		{
			for (int i = 0; i < 3; i++)
			{
				SDEdge edge(face->vertices[i], face->vertices[NEXT(i)]);
				auto iter = edgeVerts.find(edge);
				if (iter == edgeVerts.end())
				{
					SDVertex* vertex = new SDVertex();
					newVertices.push_back(vertex);

					vertex->boundary = (face->neighborFaces[i] == nullptr);
					vertex->face = face->children[3];

					if (vertex->boundary)
					{
						vertex->point = 0.5f * (edge.vertices[0]->point + edge.vertices[1]->point);
					}
					else
					{
						vertex->point = (3.0f / 8) * (edge.vertices[0]->point + edge.vertices[1]->point);

						SDVertex* C = face->getOtherVertex(edge.vertices[0], edge.vertices[1]);
						SDVertex* D = face->neighborFaces[i]->getOtherVertex(edge.vertices[0], edge.vertices[1]);
						vertex->point += (1.0f / 8) * (C->point + D->point);
					}

					edgeVerts[edge] = vertex;
				}
			}
		}

		// update old vertex face
		for (auto& vertex : v)
		{
			int index = vertex->face->getVertexIndex(vertex);
			vertex->child->face = vertex->face->children[index];
		}

		// update face neighbor 
		for (const auto& face : f)
		{
			for (int j = 0; j < 3; j++)
			{
				face->children[3]->neighborFaces[j] = face->children[NEXT(j)];
				face->children[j]->neighborFaces[NEXT(j)] = face->children[3];

				SDFace* f2 = face->neighborFaces[j];
				face->children[j]->neighborFaces[j] = f2 ? f2->children[f2->getVertexIndex(face->vertices[j])] : nullptr;

				f2 = face->neighborFaces[PREV(j)];
				face->children[j]->neighborFaces[PREV(j)] = f2 ? f2->children[f2->getVertexIndex(face->vertices[j])] : nullptr;
			}
		}

		// update face vertex
		for (auto& face : f)
		{
			for (int j = 0; j < 3; j++)
			{
				face->children[j]->vertices[j] = face->vertices[j]->child;

				SDVertex* vertex = edgeVerts[SDEdge(face->vertices[j], face->vertices[NEXT(j)])];
				face->children[j]->vertices[NEXT(j)] = vertex;
				face->children[NEXT(j)]->vertices[j] = vertex;
				face->children[3]->vertices[j] = vertex;
			}
		}

		// clear f v
		for (auto& vertex : v)
		{
			delete vertex;
		}

		for (auto& face : f)
		{
			delete face;
		}

		f = newFaces;
		v = newVertices;
	}

	//
	m_Vertex.clear();
	for (const auto& face : f)
	{
		Vec3f A = face->vertices[1]->point - face->vertices[0]->point;
		Vec3f B = face->vertices[2]->point - face->vertices[0]->point;
		Vec3f N = normalize(cross(A, B));

		for (int i = 0; i < 3; i++)
		{
			Vertex vertex;
			vertex.position = face->vertices[i]->point;
			vertex.normal = N;

			m_Vertex.push_back(vertex);
		}
	}

	// clear f v
	for (auto& vertex : v)
	{
		delete vertex;
	}

	for (auto& face : f)
	{
		delete face;
	}

	initTriangleList();
}


void Mesh::clear()
{
	for (auto triangle : m_TriangleList)
	{
		delete triangle;
	}
	m_TriangleList.clear();
}


void Mesh::updateVertex()
{
	m_Vertex.clear();

	int vSize = m_Vertices.size();
	int nSize = m_Normals.size();
	int tSize = m_TexCoords.size();

	for (const auto& face : m_Faces)
	{
		for (int i = 0; i < 3; i++)
		{
			Vertex vertex;
			if (m_FaceType == FaceType::TYPE_V)
			{
				int idx = face.faceVertex[i].vIndex;
				idx = (idx < 0) ? vSize + idx : idx - 1;
				vertex.position = m_Vertices[idx];
			}
			else if (m_FaceType == FaceType::TYPE_V_N)
			{
				int idx = face.faceVertex[i].vIndex;
				idx = (idx < 0) ? vSize + idx : idx - 1;
				vertex.position = m_Vertices[idx];

				idx = face.faceVertex[i].nIndex;
				idx = (idx < 0) ? nSize + idx : idx - 1;
				vertex.normal = m_Normals[idx];
			}
			else if (m_FaceType == FaceType::TYPE_V_T)
			{
				int idx = face.faceVertex[i].vIndex;
				idx = (idx < 0) ? vSize + idx : idx - 1;
				vertex.position = m_Vertices[idx];

				idx = face.faceVertex[i].tIndex;
				idx = (idx < 0) ? tSize + idx : idx - 1;
				vertex.texCoord = m_TexCoords[idx];
			}
			else if (m_FaceType == FaceType::TYPE_V_N_T)
			{
				int idx = face.faceVertex[i].vIndex;
				idx = (idx < 0) ? vSize + idx : idx - 1;
				vertex.position = m_Vertices[idx];

				idx = face.faceVertex[i].nIndex;
				idx = (idx < 0) ? nSize + idx : idx - 1;
				vertex.normal = m_Normals[idx];

				idx = face.faceVertex[i].tIndex;
				idx = (idx < 0) ? tSize + idx : idx - 1;
				vertex.texCoord = m_TexCoords[idx];
			}

			m_Vertex.push_back(std::move(vertex));
		}

		if (m_FaceType == FaceType::TYPE_V || m_FaceType == FaceType::TYPE_V_T)
		{
			int vertexSize = m_Vertex.size();

			Vec3f A = m_Vertex[vertexSize - 2].position - m_Vertex[vertexSize - 3].position;
			Vec3f B = m_Vertex[vertexSize - 1].position - m_Vertex[vertexSize - 3].position;
			Vec3f N = normalize(cross(A, B));

			for (int i = vertexSize - 3; i < vertexSize; i++)
			{
				m_Vertex[i].normal = N;
			}
		}
	}
}

void Mesh::initTriangleList()
{
	clear();

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
