#pragma once

#include "Material.h"

class DiffuseLight : public Material
{
public:
	DiffuseLight();
	DiffuseLight(const Vec3f& emitColor);
	virtual ~DiffuseLight();

	virtual bool hasEmission() const override;
	virtual Vec3f getEmission() const override;

	virtual bool scatter(const Vec3f& wi, const Vertex& vertex, Ray& scattered) const override;

private:
	Vec3f m_EmitColor;
};