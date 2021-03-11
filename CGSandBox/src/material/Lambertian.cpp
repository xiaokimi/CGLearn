#include "Lambertian.h"

Lambertian::Lambertian()
{

}

Lambertian::Lambertian(const std::shared_ptr<Texture>& albedo)
: m_Albedo(albedo)
{

}

Lambertian::~Lambertian()
{

}

bool Lambertian::scatter(const Vec3f& wi, const Vertex& vertex, Ray& scattered) const
{
	Vec3f N = vertex.normal;
	if (dot(-wi, N) > 0)
	{
		float z = dis(gen);
		float r = std::sqrt(1.0f - z * z);
		float phi = 2 * M_PI * dis(gen);
		Vec3f localDir(r * std::cosf(phi), r * std::sinf(phi), z);

		Vec3f wo = toWorld(localDir, N);
		scattered = Ray(vertex.position, wo);
		return true;
	}

	return false;
}

Vec3f Lambertian::evaluate(const Vec3f& wi, const Vec3f& wo, const Vertex& vertex) const
{
	Vec3f N = vertex.normal;
	float costheta = dot(wo, N);

	if (costheta > 0.0f)
	{
		return getColor(vertex) / M_PI;
	}

	return Vec3f(0.0f);
}

Vec3f Lambertian::toWorld(const Vec3f& localDir, const Vec3f& N) const
{
	Vec3f B, C;
	if (std::fabs(N[0]) > std::fabs(N[1])) 
	{
		float invLen = 1.0f / std::sqrt(N[0] * N[0] + N[2] * N[2]);
		C = Vec3f(N[3] * invLen, 0.0f, -N[0] * invLen);
	}
	else 
	{
		float invLen = 1.0f / std::sqrt(N[1] * N[1] + N[2] * N[2]);
		C = Vec3f(0.0f, N[2] * invLen, -N[1] * invLen);
	}
	B = cross(C, N);
	return localDir[0] * B + localDir[1] * C + localDir[2] * N;
}

Vec3f Lambertian::getColor(const Vertex& vertex) const
{
	if (m_Albedo)
	{
		return m_Albedo->getColor(vertex);
	}
	
	return Vec3f(0.0f);
}
