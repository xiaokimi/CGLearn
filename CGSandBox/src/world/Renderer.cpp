#include "Renderer.h"
#include "../material/Material.h"

Renderer::Renderer()
: m_Width(100)
, m_Height(100)
, m_RussianRoulette(0.8f)
, m_ThreadExit(false)
{
	m_FrameBuffer.resize(m_Width * m_Height);
	initPixelUVs();
	initThread();
}

Renderer::Renderer(int width, int height)
: m_Width(width)
, m_Height(height)
, m_RussianRoulette(0.8f)
, m_ThreadExit(false)
{
	m_FrameBuffer.resize(m_Width * m_Height);
	initPixelUVs();
	initThread();
}

Renderer::~Renderer()
{
	for (auto t : m_Threads)
	{
		delete t;
	}
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

				TaskData taskData = { index, ray, (Scene*)(&scene) };
				auto func = std::bind(&Renderer::castRayTask, this, std::placeholders::_1);
				Task task = { taskData, func };
				addTask(task);
			}

			index++;
		}
	}

	m_ThreadExit = true;
	for (const auto& t : m_Threads)
	{
		t->join();
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


void Renderer::initThread()
{
	for (int i = 0; i < MAX_THREAD_SIZE; i++)
	{
		std::thread* t = new std::thread([=] {
			run();
		});
		m_Threads.push_back(t);
	}
}

Vec3f Renderer::castRay(const Scene& scene, const Ray& ray) const
{
	HitRecord record;
	if (scene.hit(ray, 0.001f, kInfinity, record))
	{
		if (record.material->hasEmission())
		{
			return record.material->getEmission();
		}

		Vec3f P = record.vertex.position;
		Vec3f N = record.vertex.normal;

		HitRecord lightRecord;
		float lightPdf = 0.0f;
		scene.sampleLight(lightRecord, lightPdf);

		Vec3f obj2light = lightRecord.vertex.position - P;
		Vec3f wo = normalize(obj2light);

		Vec3f L_dir = Vec3f(0.0f);

		float theta2 = dot(-wo, lightRecord.vertex.normal);
		if (theta2 > 0)
		{
			Ray testRay(P, wo);
			HitRecord testRecord;
			if (scene.hit(testRay, 0.001f, kInfinity, testRecord))
			{
				if (lengthSquared(testRecord.vertex.position - P) - lengthSquared(obj2light) > -0.001f)
				{
					Vec3f Li = lightRecord.material->getEmission();
					Vec3f f_r = record.material->evaluate(ray.getDirection(), wo, record.vertex);

					L_dir = Li * f_r * dot(wo, N) * theta2 / lengthSquared(obj2light) / lightPdf;
				}
			}
		}

		if (dis(gen) > m_RussianRoulette)
		{
			return L_dir;
		}

		Vec3f L_indir = Vec3f(0.0f);

		Ray scattered;
		if (record.material->scatter(ray.getDirection(), record.vertex, scattered))
		{
			float pdf = 0.5f / M_PI;
			Vec3f f_r = record.material->evaluate(ray.getDirection(), scattered.getDirection(), record.vertex);

			L_indir = castRay(scene, scattered) * f_r * dot(scattered.getDirection(), N) / pdf / m_RussianRoulette;
		}

		return L_dir + L_indir;
	}

	return Vec3f(0.0f);
}

void Renderer::run()
{
	while (true)
	{
		std::unique_lock l(m_Lock);

		if (!m_TaskQueue.empty())
		{
			Task t = m_TaskQueue.front();
			m_TaskQueue.pop();
			l.unlock();

			t.func(t.data);
			m_NotFull.notify_all();
		}
		else if (m_ThreadExit)
		{
			break;
		}
		else
		{
			m_NotEmpty.wait_for(l, std::chrono::milliseconds(50));
		}
	}
}

void Renderer::addTask(const Task& task)
{
	while (true)
	{
		std::unique_lock l(m_Lock);
		if (m_TaskQueue.size() < MAX_QUEUE_SIZE)
		{
			m_TaskQueue.push(task);
			l.unlock();

			m_NotEmpty.notify_all();
			break;
		}
		else
		{
			m_NotFull.wait(l);
		}
	}
}

void Renderer::castRayTask(const TaskData& taskData)
{
	Vec3f color = castRay(*(taskData.scene), taskData.ray) / m_PixelUVs.size();

	std::unique_lock l(m_BuffLock);
	m_FrameBuffer[taskData.buffIndex] += color;
	l.unlock();
}
