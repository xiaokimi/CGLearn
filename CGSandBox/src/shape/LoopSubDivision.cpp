#include "global.h"
#include "LoopSubDivision.h"

SDVertex::SDVertex(const Vec3f& p /* = Vec3f(0.0f) */)
: point(p)
, child(nullptr)
, face(nullptr)
, boundary(false)
{

}

int SDVertex::getValence() const
{
	SDFace* f = face;
	int u = 1;

	if (!boundary)
	{
		while ((f = f->getNextFace(this)) != face)
		{
			u++;
		}
	}
	else
	{
		while ((f = f->getNextFace(this)) != nullptr)
		{
			u++;
		}

		f = face;
		while ((f = f->getPrevFace(this)) != nullptr)
		{
			u++;
		}
	}

	return u;
}

Vec3f SDVertex::getOldAdjustPosition() const
{
	std::vector<Vec3f> points;
	getSurroundPoints(points);

	Vec3f position;
	if (boundary)
	{
		position = (3.0f / 4) * point;
		position += (1.0f / 8) * (points[0] + points[points.size() - 1]);
	}
	else
	{
		//int valence = getValence();
		int valence = points.size();
		float beta = (valence == 3) ? 3.0f / 16 : 3.0f / (8 * valence);

		position = (1.0f - valence * beta) * point;
		for (const auto& p : points)
		{
			position += beta * p;
		}
	}

	return position;
}

void SDVertex::getSurroundPoints(std::vector<Vec3f>& points) const
{
	if (!boundary)
	{
		SDFace* f = face;
		do
		{
			points.push_back(f->getNextVertex(this)->point);
			f = f->getNextFace(this);
		} while (f != face);
	}
	else
	{
		SDFace* f = face;
		points.push_back(f->getNextVertex(this)->point);

		while ((f = f->getNextFace(this)) != nullptr)
		{
			points.push_back(f->getNextVertex(this)->point);
		}

		f = face;
		while ((f = f->getPrevFace(this)) != nullptr)
		{
			points.push_back(f->getPrevVertex(this)->point);
		}
	}
}

//
SDFace::SDFace()
{
	for (int i = 0; i < 3; i++)
	{
		vertices[i] = nullptr;
		neighborFaces[i] = nullptr;
	}

	for (int i = 0; i < 4; i++)
	{
		children[i] = nullptr;
	}
}

int SDFace::getVertexIndex(const SDVertex* vertex) const
{
	for (int i = 0; i < 3; i++)
	{
		if (vertices[i] == vertex)
		{
			return i;
		}
	}

	return -1;
}

SDVertex* SDFace::getNextVertex(const SDVertex* vertex) const
{
	int idx = getVertexIndex(vertex);
	return vertices[NEXT(idx)];
}

SDVertex* SDFace::getPrevVertex(const SDVertex* vertex) const
{
	int idx = getVertexIndex(vertex);
	return vertices[PREV(idx)];
}

SDVertex* SDFace::getOtherVertex(const SDVertex* v0, const SDVertex* v1) const
{
	for (int i = 0; i < 3; i++)
	{
		if (vertices[i] != v0 && vertices[i] != v1)
		{
			return vertices[i];
		}
	}

	return nullptr;
}

SDFace* SDFace::getNextFace(const SDVertex* vertex) const
{
	int idx = getVertexIndex(vertex);
	return neighborFaces[idx];
}

SDFace* SDFace::getPrevFace(const SDVertex* vertex) const
{
	int idx = getVertexIndex(vertex);
	return neighborFaces[PREV(idx)];
}

//
SDEdge::SDEdge(SDVertex* v0 /* = nullptr */, SDVertex* v1 /* = nullptr */)
: face0index(-1)
{
	vertices[0] = std::min(v0, v1);
	vertices[1] = std::max(v0, v1);

	faces[0] = nullptr;
	faces[1] = nullptr;
}

bool SDEdge::operator<(const SDEdge& edge) const
{
	if (vertices[0] == edge.vertices[0])
	{
		return vertices[1] < edge.vertices[1];
	}

	return vertices[0] < edge.vertices[0];
}

bool SDEdge::operator==(const SDEdge& edge) const
{
	if (vertices[0] == edge.vertices[0] && vertices[1] == edge.vertices[1])
	{
		return true;
	}

	return false;
}
