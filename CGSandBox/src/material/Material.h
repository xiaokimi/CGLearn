#pragma once

#include "global.h"
#include "../geometry/Ray.h"
#include "../texture/Texture.h"


class Material
{
public:
	virtual bool hasEmission() const
	{
		return false;
	}

	virtual Vec3f getEmission() const
	{
		return Vec3f(0.0f);
	}

	virtual bool scatter(const Vec3f& wi, const Vertex& vertex, Ray& scattered) const = 0;

	virtual Vec3f evaluate(const Vec3f& wi, const Vec3f& wo, const Vertex& vertex) const
	{
		return Vec3f(0.0f);
	}
};