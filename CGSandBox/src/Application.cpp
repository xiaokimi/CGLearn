#include "world/Renderer.h"
#include "shape/Sphere.h"

void saveToFile(const Renderer& renderer, const char* filePath);

int main()
{
	int width = 400;
	int height = 400;

	Vec3f lookfrom(0, 0, 5.0f);
	Vec3f lookat(0);
	Vec3f vup(0, 1.0f, 0);
	float vfov = 90.0f;
	float aspect = float(width) / float(height);

	Camera camera(lookfrom, lookat, vup, vfov, aspect);
	Renderer renderer(width, height);
	Scene scene(width, height);

	std::vector<Object*> objectList;
	objectList.push_back(new Sphere(Vec3f(-1.0f, 0, -1.0f), 1.0f));
	objectList.push_back(new Sphere(Vec3f(-0.9f, 0, -1.0f), 1.0f));
	objectList.push_back(new Sphere(Vec3f(-0.8f, 0, -1.0f), 1.0f));
	objectList.push_back(new Sphere(Vec3f(-0.7f, 0, -1.0f), 1.0f));

	objectList.push_back(new Sphere(Vec3f(2.0f, 0, -1.0f), 1.0f));

	scene.setObjectList(objectList);

	renderer.render(scene, camera);
	saveToFile(renderer, "BVH.ppm");

	std::cin.get();
}

void saveToFile(const Renderer& renderer, const char* filePath)
{
	float gamma = 1.0f / M_GAMMA;
	int width = renderer.getWidth();
	int height = renderer.getHeight();
	const std::vector<Vec3f>& frameBuffer = renderer.getFrameBuffer();

	FILE* fp;
	errno_t err = fopen_s(&fp, filePath, "wb");
	fprintf(fp, "P3\n%d %d\n255", width, height);
	for (const auto& color : frameBuffer)
	{
		int R = lerp<float>(0.0f, 1.0f, std::powf(color[0], gamma)) * 255.0f;
		int G = lerp<float>(0.0f, 1.0f, std::powf(color[1], gamma)) * 255.0f;
		int B = lerp<float>(0.0f, 1.0f, std::powf(color[2], gamma)) * 255.0f;
		fprintf(fp, "\n%d %d %d", R, G, B);
	}
	fclose(fp);
}