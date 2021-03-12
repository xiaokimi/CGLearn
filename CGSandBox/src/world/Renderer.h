#pragma once

#include "Scene.h"
#include "Camera.h"

constexpr int MAX_QUEUE_SIZE = 1000;
constexpr int MAX_THREAD_SIZE = 4;

struct TaskData
{
	int buffIndex;

	Ray ray;
	Scene* scene;
};

struct Task
{
	TaskData data;
	std::function<void(TaskData)> func;
};

class Renderer
{
public:
	Renderer();
	Renderer(int width, int height);
	virtual ~Renderer();

	int getWidth() const;
	int getHeight() const;
	[[nodiscard]] const std::vector<Vec3f>& getFrameBuffer() const;

	void render(const Scene& scene, const Camera& camera);

private:
	void initPixelUVs();
	Vec3f castRay(const Scene& scene, const Ray& ray, int depth) const;

	void run();
	void addTask(const Task& task);
	void castRayTask(const TaskData& taskData);

private:
	int m_Width;
	int m_Height;
	float m_RussianRoulette;

	std::vector<Vec3f> m_FrameBuffer;
	std::vector<Vec2f> m_PixelUVs;

	bool m_ThreadExit;
	std::queue<Task> m_TaskQueue;

	std::mutex m_TaskLock;
	std::mutex m_BuffLock;

	std::condition_variable m_ProduceCondition;
	std::condition_variable m_ConsumeCondition;
};