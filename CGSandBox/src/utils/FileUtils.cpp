#include "FileUtils.h"
#include "../shape/Object.h"

bool FileUtils::loadOBJFile(const std::string& filePath, std::vector<Vertex>& vertex)
{
	if (filePath.substr(filePath.size() - 4, 4) != ".obj")
	{
		return false;
	}

	std::ifstream file(filePath);
	if (!file.is_open())
	{
		return false;
	}

	std::vector<Vec3f> positions;
	std::vector<Vec3f> normals;
	std::vector<Vec2f> texCoords;

	std::string curline;
	while (std::getline(file, curline))
	{
		std::string tag = StringUtils::firstToken(curline);
		if (tag == "v")
		{
			std::vector<std::string> spos;
			StringUtils::split(StringUtils::tail(curline), spos, " ");

			float x = std::stof(spos[0]);
			float y = std::stof(spos[1]);
			float z = std::stof(spos[2]);

			positions.push_back(Vec3f(x, y, z));
		}
		else if (tag == "vn")
		{
			std::vector<std::string> snor;
			StringUtils::split(StringUtils::tail(curline), snor, " ");

			float x = std::stof(snor[0]);
			float y = std::stof(snor[1]);
			float z = std::stof(snor[2]);

			normals.push_back(Vec3f(x, y, z));
		}
		else if (tag == "vt")
		{
			std::vector<std::string> stex;
			StringUtils::split(StringUtils::tail(curline), stex, " ");

			float x = std::stof(stex[0]);
			float y = std::stof(stex[1]);

			texCoords.push_back(Vec2f(x, y));
		}
		else if (tag == "f")
		{
			FileUtils::genVerticesFromRawOBJ(curline, positions, normals, texCoords, vertex);
		}
	}

	file.close();
	return true;
}

void FileUtils::genVerticesFromRawOBJ(const std::string& in, const std::vector<Vec3f>& positions, const std::vector<Vec3f>& normals, const std::vector<Vec2f>& texCoords, std::vector<Vertex>& vertex)
{
	std::vector<std::string> sface, svert;
	StringUtils::split(StringUtils::tail(in), sface, " ");
	bool noNormal = false;

	for (int i = 0; i < int(sface.size()); i++)
	{
		int vtype = 0;

		StringUtils::split(sface[i], svert, "/");
		if (svert.size() == 1)
		{
			vtype = 1;
		}
		else if (svert.size() == 2)
		{
			vtype = 2;
		}
		else if (svert.size() == 3)
		{
			if (svert[1] != "")
			{
				vtype = 4;
			}
			else
			{
				vtype = 3;
			}
		}

		noNormal = (vtype == 1 || vtype == 2);

		if (vtype == 1)
		{
			Vertex v;
			v.position = StringUtils::getElement(positions, svert[0]);

			vertex.push_back(v);
		}
		else if (vtype == 2)
		{
			Vertex v;
			v.position = StringUtils::getElement(positions, svert[0]);
			v.texCoord = StringUtils::getElement(texCoords, svert[1]);

			vertex.push_back(v);
		}
		else if (vtype == 3)
		{
			Vertex v;
			v.position = StringUtils::getElement(positions, svert[0]);
			v.normal = StringUtils::getElement(normals, svert[2]);

			vertex.push_back(v);
		}
		else if (vtype == 4)
		{
			Vertex v;
			v.position = StringUtils::getElement(positions, svert[0]);
			v.texCoord = StringUtils::getElement(texCoords, svert[1]);
			v.normal = StringUtils::getElement(normals, svert[2]);

			vertex.push_back(v);
		}
	}

	if (noNormal)
	{
		int nSize = vertex.size();

		Vec3f A = vertex[nSize - 2].position - vertex[nSize - 3].position;
		Vec3f B = vertex[nSize - 1].position - vertex[nSize - 3].position;
		Vec3f N = normalize(cross(A, B));

		for (int i = nSize - 1; i >= int(nSize - sface.size()); i--)
		{
			vertex[i].normal = N;
		}
	}
}
