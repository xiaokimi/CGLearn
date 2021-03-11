#pragma once

#include "../shape/Object.h"

class Texture
{
public:
	virtual Vec3f getColor(const Vertex& vertex) const = 0;
};