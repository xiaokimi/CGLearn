#pragma once

#include "Scene.h"
#include "Camera.h"

#define  MAX_QUEUE_SIZE 1000
#define  MAX_THREAD_SIZE 4

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
	void initThread();

	Vec3f castRay(const Scene& scene, const Ray& ray) const;

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

	std::mutex m_Lock;
	std::mutex m_BuffLock;
	std::condition_variable m_NotEmpty;
	std::condition_variable m_NotFull;
	std::vector<std::thread*> m_Threads;
};