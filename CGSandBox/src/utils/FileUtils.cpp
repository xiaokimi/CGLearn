#include "FileUtils.h"
#include "../shape/Mesh.h"

bool FileUtils::loadOBJFile(const std::string& filePath, std::vector<Vec3f>& vertices, std::vector<Vec3f>& normals, std::vector<Vec2f>& texCoords, std::vector<Face>& faces, FaceType& faceType)
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

			vertices.push_back(Vec3f(x, y, z));
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
			FileUtils::genVerticesFromRawOBJ(curline, faces, faceType);
		}
	}

	file.close();
	return true;
}

void FileUtils::genVerticesFromRawOBJ(const std::string& in, std::vector<Face>& faces, FaceType& faceType)
{
	std::vector<std::string> sface, svert;
	StringUtils::split(StringUtils::tail(in), sface, " ");

	Face face;
	for (int i = 0; i < int(sface.size()) && i < 3; i++)
	{
		StringUtils::split(sface[i], svert, "/");
		if (svert.size() == 1)
		{
			faceType = FaceType::TYPE_V;
		}
		else if (svert.size() == 2)
		{
			faceType = FaceType::TYPE_V_T;
		}
		else if (svert.size() == 3)
		{
			if (svert[1] != "")
			{
				faceType = FaceType::TYPE_V_N_T;
			}
			else
			{
				faceType = FaceType::TYPE_V_N;
			}
		}

		//
		if (faceType == FaceType::TYPE_V)
		{
			face.faceVertex[i].vIndex = std::stoi(svert[0]);
		}
		else if (faceType == FaceType::TYPE_V_T)
		{
			face.faceVertex[i].vIndex = std::stoi(svert[0]);
			face.faceVertex[i].tIndex = std::stoi(svert[1]);
		}
		else if (faceType == FaceType::TYPE_V_N)
		{
			face.faceVertex[i].vIndex = std::stoi(svert[0]);
			face.faceVertex[i].nIndex = std::stoi(svert[2]);
		}
		else if (faceType == FaceType::TYPE_V_N_T)
		{
			face.faceVertex[i].vIndex = std::stoi(svert[0]);
			face.faceVertex[i].tIndex = std::stoi(svert[1]);
			face.faceVertex[i].nIndex = std::stoi(svert[2]);
		}
	}

	faces.push_back(std::move(face));
}
