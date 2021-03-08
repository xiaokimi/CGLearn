#include "Renderer.h"

Renderer::Renderer()
: m_Width(100)
, m_Height(100)
, m_MaxDepth(5)
{
	m_FrameBuffer.resize(m_Width * m_Height);
	initPixelUVs();
}

Renderer::Renderer(int width, int height)
: m_Width(width)
, m_Height(height)
, m_MaxDepth(5)
{
	m_FrameBuffer.resize(m_Width * m_Height);
	initPixelUVs();
}

Renderer::~Renderer()
{

}

int Renderer::getWidth() const
{
	return m_Width;
}

int Renderer::getHeight() const
{
	return m_Height;
}


const std::vector<Vec3f>& Renderer::getFrameBuffer() const
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
			for (const Vec2f& offset : m_PixelUVs)
			{
				float s = (u + offset[0]) / m_Width;
				float t = (v + offset[1]) / m_Height;
				Ray ray = camera.getRay(s, t);

				color += castRay(scene, ray, 0);
			}

			m_FrameBuffer[index++] = color / m_PixelUVs.size();
		}
	}
}

void Renderer::initPixelUVs()
{
	int sampleCount = 5;
	for (int v = sampleCount - 1; v > 0; v--)
	{
		for (int u = 1; u < sampleCount; u++)
		{
			m_PixelUVs.push_back(Vec2f(u * 1.0f / sampleCount, v * 1.0f / sampleCount));
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
