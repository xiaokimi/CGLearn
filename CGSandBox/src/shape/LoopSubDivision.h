#pragma once

#include "../geometry/Vector.h"

#define NEXT(i) (((i) + 1) % 3)
#define PREV(i) (((i) + 2) % 3)

class SDFace;

struct SDVertex
{
public:
	SDVertex(const Vec3f& p = Vec3f(0.0f));

	int getValence() const;

	Vec3f getOldAdjustPosition() const;
	void getSurroundPoints(std::vector<Vec3f>& points) const;

	Vec3f getNewPosition(int index) const;

public:
	Vec3f point;
	SDVertex* child;
	SDFace* face;
	bool boundary;
};

struct SDFace
{
public:
	SDFace();

	int getVertexIndex(const SDVertex* vertex) const;

	SDVertex* getNextVertex(const SDVertex* vertex) const;
	SDVertex* getPrevVertex(const SDVertex* vertex) const;
	SDVertex* getOtherVertex(const SDVertex* v0, const SDVertex* v1) const;

	SDFace* getNextFace(const SDVertex* vertex) const;
	SDFace* getPrevFace(const SDVertex* vertex) const;

public:
	SDVertex* vertices[3];
	SDFace* neighborFaces[3];
	SDFace* children[4];
};

struct SDEdge
{
public:
	SDEdge(SDVertex* v0 = nullptr, SDVertex* v1 = nullptr);

	bool operator<(const SDEdge& edge) const;
	bool operator==(const SDEdge& edge) const;

public:
	SDVertex* vertices[2];
	SDFace* faces[2];
	int face0index;
};