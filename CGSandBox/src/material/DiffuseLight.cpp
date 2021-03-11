#include "DiffuseLight.h"

DiffuseLight::DiffuseLight()
{

}

DiffuseLight::DiffuseLight(const Vec3f& emitColor)
: m_EmitColor(emitColor)
{

}

DiffuseLight::~DiffuseLight()
{

}

bool DiffuseLight::hasEmission() const
{
	return true;
}

Vec3f DiffuseLight::getEmission() const
{
	return m_EmitColor;
}

bool DiffuseLight::scatter(const Vec3f& wi, const Vertex& vertex, Ray& scattered) const
{
	return false;
}
