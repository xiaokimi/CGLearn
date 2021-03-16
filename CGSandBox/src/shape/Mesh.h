#pragma once

#include "global.h"
#include "Triangle.h"

enum class FaceType
{
	TYPE_NONE     = 0,
	TYPE_V        = 1,
	TYPE_V_N      = 2,
	TYPE_V_T      = 3,
	TYPE_V_N_T    = 4
};

struct FaceVertex
{
	unsigned int vIndex;
	unsigned int nIndex;
	unsigned int tIndex;
};

struct Face
{
	FaceVertex faceVertex[3];
};

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

public:
	void loopSubDivide(int level);

private:
	void clear();
	void updateVertex();
	void initTriangleList();

private:
	float m_Area;

	std::vector<Vec3f> m_Vertices;
	std::vector<Vec3f> m_Normals;
	std::vector<Vec2f> m_TexCoords;

	FaceType m_FaceType;
	std::vector<Face> m_Faces;

	std::shared_ptr<Material> m_Material;

	std::vector<Vertex> m_Vertex;
	std::vector<Triangle*> m_TriangleList;

	int m_SubDivideLevel;
};