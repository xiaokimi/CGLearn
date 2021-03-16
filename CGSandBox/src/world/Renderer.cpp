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
}

Renderer::Renderer(int width, int height)
: m_Width(width)
, m_Height(height)
, m_RussianRoulette(0.8f)
, m_ThreadExit(false)
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
	m_ThreadExit = false;
	std::thread threads[MAX_THREAD_SIZE];
	for (int i = 0; i < MAX_THREAD_SIZE; i++)
	{
		threads[i] = std::thread(std::bind(&Renderer::run, this));
	}

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
	for (int i = 0; i < MAX_THREAD_SIZE; i++)
	{
		threads[i].join();
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

Vec3f Renderer::castRay(const Scene& scene, const Ray& ray, int depth) const
{
	HitRecord record;
	if (scene.hit(ray, 0.001f, kInfinity, record))
	{
		return Vec3f(1.0f);


		if (record.material->hasEmission())
		{
			return record.material->getEmission();
		}

		Vec3f wi = ray.getDirection();
		Vec3f P = record.vertex.position;
		Vec3f N = record.vertex.normal;

		HitRecord lightRecord;
		float lightPdf = 0.0f;
		scene.sampleLight(lightRecord, lightPdf);

		Vec3f obj2light = lightRecord.vertex.position - P;
		Vec3f wo = normalize(obj2light);

		Vec3f L_dir = Vec3f(0.0f);

		float theta2 = dot(-wo, lightRecord.vertex.normal);
		if (theta2 > 0.0f)
		{
			Ray testRay(P, wo);
			HitRecord testRecord;
			if (scene.hit(testRay, 0.001f, kInfinity, testRecord))
			{
				if (lengthSquared(testRecord.vertex.position - P) - lengthSquared(obj2light) > -0.001f)
				{
					Vec3f Li = lightRecord.material->getEmission();
					Vec3f f_r = record.material->evaluate(wi, wo, record.vertex);

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
		if (record.material->scatter(wi, record.vertex, scattered))
		{
			float pdf = 0.5f / M_PI;
			Vec3f f_r = record.material->evaluate(wi, scattered.getDirection(), record.vertex);

			L_indir = castRay(scene, scattered, depth + 1) * f_r * dot(scattered.getDirection(), N) / pdf / m_RussianRoulette;
		}

		return L_dir + L_indir;
	}

	return Vec3f(0.0f);
}

void Renderer::run()
{
	while (true)
	{
		std::unique_lock l(m_TaskLock);

		if (!m_TaskQueue.empty())
		{
			Task task = m_TaskQueue.front();
			m_TaskQueue.pop();
			l.unlock();

			task.func(task.data);
			m_ProduceCondition.notify_all();
		}
		else if (m_ThreadExit)
		{
			break;
		}
		else
		{
			m_ConsumeCondition.wait_for(l, std::chrono::milliseconds(50));
		}
	}
}

void Renderer::addTask(const Task& task)
{
	while (true)
	{
		std::unique_lock l(m_TaskLock);
		if (m_TaskQueue.size() < MAX_QUEUE_SIZE)
		{
			m_TaskQueue.push(task);
			l.unlock();

			m_ConsumeCondition.notify_all();
			break;
		}
		else
		{
			m_ProduceCondition.wait(l);
		}
	}
}

void Renderer::castRayTask(const TaskData& taskData)
{
	Vec3f color = castRay(*(taskData.scene), taskData.ray, 0) / m_PixelUVs.size();

	std::unique_lock l(m_BuffLock);
	m_FrameBuffer[taskData.buffIndex] += color;
}
