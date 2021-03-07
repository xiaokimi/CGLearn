#pragma once

#include "Scene.h"
#include "Camera.h"

class Renderer
{
public:
	Renderer();
	Renderer(int width, int height);
	virtual ~Renderer();

	int getWidth() const;
	int getHeight() const;
	const Vec3f* getFrameBuffer() const;

	void render(const Scene& scene, const Camera& camera);

private:
	void initPixelUVs();
	Vec3f castRay(const Scene& scene, const Ray& ray, const int& depth) const;

private:
	int m_Width;
	int m_Height;
	int m_MaxDepth;

	Vec3f* m_FrameBuffer;

	int m_PixelRaySize;
	Vec2f* m_PixelUVs;
};