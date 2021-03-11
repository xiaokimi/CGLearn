#include "ConstantTexture.h"
#include "../material/Material.h"

ConstantTexture::ConstantTexture()
: m_Value(1.0f)
{

}

ConstantTexture::ConstantTexture(const Vec3f& value)
: m_Value(value)
{

}

ConstantTexture::~ConstantTexture()
{

}

Vec3f ConstantTexture::getColor(const Vertex& vertex) const
{
	return m_Value;
}
