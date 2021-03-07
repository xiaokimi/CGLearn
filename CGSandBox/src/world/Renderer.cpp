#include "cgpch.h"
#include "Renderer.h"

Renderer::Renderer()
: m_Width(100)
, m_Height(100)
, m_MaxDepth(5)
{
	m_FrameBuffer = new Vec3f[m_Width * m_Height];
	initPixelUVs();
}

Renderer::Renderer(int width, int height)
: m_Width(width)
, m_Height(height)
, m_MaxDepth(5)
{
	m_FrameBuffer = new Vec3f[m_Width * m_Height];
	initPixelUVs();
}

Renderer::~Renderer()
{
	delete[] m_FrameBuffer;
	delete[] m_PixelUVs;
}

int Renderer::getWidth() const
{
	return m_Width;
}

int Renderer::getHeight() const
{
	return m_Height;
}

const Vec3f* Renderer::getFrameBuffer() const
{
	return m_FrameBuffer;
}

void Renderer::render(const Scene& scene, const Camera& camera)
{
	int index = 0;
	for (int v = m_Height - 1; v >= 0; v--)
	{
		for (int u = 0; u < m_Width; u++)
		{
			Vec3f color;
			for (int i = 0; i < m_PixelRaySize; i++)
			{
				float s = (u + m_PixelUVs[i][0]) / m_Width;
				float t = (v + m_PixelUVs[i][0]) / m_Height;
				Ray ray = camera.getRay(s, t);

				color += castRay(scene, ray, 0);
			}

			m_FrameBuffer[index++] = color / m_PixelRaySize;
		}
	}
}

void Renderer::initPixelUVs()
{
	int sampleCount = 5;
	m_PixelRaySize = (sampleCount - 1) * (sampleCount - 1);

	m_PixelUVs = new Vec2f[m_PixelRaySize];

	int i = 0;
	for (int v = sampleCount - 1; v > 0; v--)
	{
		for (int u = 1; u < sampleCount; u++)
		{
			m_PixelUVs[i++] = Vec2f(u * 1.0f / sampleCount, v * 1.0f / sampleCount);
		}
	}
}

Vec3f Renderer::castRay(const Scene& scene, const Ray& ray, const int& depth) const
{
	HitRecord record;
	if (scene.hit(ray, 0, kInfinity, record))
	{
		return Vec3f(1, 0, 0);
	}

	return Vec3f(1);
}
