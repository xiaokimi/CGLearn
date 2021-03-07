#include "cgpch.h"
#include "Camera.h"

Camera::Camera()
: m_Origin(0, 0, -1)
, m_LowerLeftCorner(-1, -1, 0)
, m_Horizontal(2, 0, 0)
, m_Vertical(0, 2, 0)
{

}

Camera::Camera(const Vec3f& lookfrom, const Vec3f& lookat, const Vec3f& vup, float vfov, float aspect)
{
	float theta = vfov * M_PI / 180;
	float halfHeight = std::tanf(theta / 2);
	float halfWidth = halfHeight * aspect;

	m_Origin = lookfrom;

	Vec3f w = normalize(lookfrom - lookat);
	Vec3f u = normalize(cross(vup, w));
	Vec3f v = cross(w, u);

	m_LowerLeftCorner = m_Origin - halfWidth * u - halfHeight * v - w;
	m_Horizontal = 2 * halfWidth * u;
	m_Vertical = 2 * halfHeight * v;
}

Camera::~Camera()
{

}

Ray Camera::getRay(float s, float t) const
{
	Vec3f direction = m_LowerLeftCorner + m_Horizontal * s + m_Vertical * t - m_Origin;
	return Ray(m_Origin, direction);
}
