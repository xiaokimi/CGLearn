#pragma once

#include "Material.h"

class Lambertian : public Material
{
public:
	Lambertian();
	Lambertian(const std::shared_ptr<Texture>& albedo);
	virtual ~Lambertian();

	virtual bool scatter(const Vec3f& wi, const Vertex& vertex, Ray& scattered) const override;
	virtual Vec3f evaluate(const Vec3f& wi, const Vec3f& wo, const Vertex& vertex) const override;

private:
	Vec3f toWorld(const Vec3f& a, const Vec3f& N) const;

	Vec3f getColor(const Vertex& vertex) const;

private:
	std::shared_ptr<Texture> m_Albedo;
};