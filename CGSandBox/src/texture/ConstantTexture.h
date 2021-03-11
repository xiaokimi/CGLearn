#pragma once

#include "Texture.h"

class ConstantTexture : public Texture
{
public:
	ConstantTexture();
	ConstantTexture(const Vec3f& value);
	virtual ~ConstantTexture();

	virtual Vec3f getColor(const Vertex& vertex) const override;

private:
	Vec3f m_Value;
};