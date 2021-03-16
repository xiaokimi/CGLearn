#pragma once

#include "StringUtils.h"
#include "../geometry/Vector.h"

struct Face;
enum class FaceType;

class FileUtils
{
public:
	static bool loadOBJFile(const std::string& filePath, std::vector<Vec3f>& vertices, std::vector<Vec3f>& normals, std::vector<Vec2f>& texCoords, std::vector<Face>& faces, FaceType& faceType);

private:
	static void genVerticesFromRawOBJ(const std::string& in, std::vector<Face>& faces, FaceType& faceType);
};