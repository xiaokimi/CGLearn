#pragma once

#include "../geometry/Ray.h"

class Camera
{
public:
	Camera();
	Camera(const Vec3f& lookfrom, const Vec3f& lookat, const Vec3f& vup, float vfov, float aspect);
	virtual ~Camera();

	Ray getRay(float s, float t) const;

private:
	Vec3f m_Origin;
	Vec3f m_LowerLeftCorner;
	Vec3f m_Horizontal;
	Vec3f m_Vertical;

};