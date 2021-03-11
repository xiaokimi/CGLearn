#pragma once

#include "StringUtils.h"
#include "../geometry/Vector.h"

struct Vertex;

class FileUtils
{
public:
	static bool loadOBJFile(const std::string& filePath, std::vector<Vertex>& vertex);

private:
	static void genVerticesFromRawOBJ(const std::string& in, const std::vector<Vec3f>& positions, const std::vector<Vec3f>& normals, const std::vector<Vec2f>& texCoords, std::vector<Vertex>& vertex);
};