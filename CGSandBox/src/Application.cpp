#include "world/Renderer.h"
#include "shape/Mesh.h"
#include "material/Lambertian.h"
#include "material/DiffuseLight.h"
#include "texture/ConstantTexture.h"
#include "geometry/Matrix.h"

void saveToFile(const Renderer& renderer, const char* filePath);

int main()
{
	int width = 784;
	int height = 784;

	Vec3f lookfrom(278.0f, 273.0f, -800.0f);
	Vec3f lookat(278.0f, 273.0f, 0.0f);
	Vec3f vup(0.0f, 1.0f, 0.0f);
	float vfov = 40.0f;
	float aspect = float(width) / float(height);

	Camera camera(lookfrom, lookat, vup, vfov, aspect);
	Renderer renderer(width, height);
	Scene scene(width, height);

	std::shared_ptr<Texture> redConstant = std::make_shared<ConstantTexture>(Vec3f(0.63f, 0.065f, 0.05f));
	std::shared_ptr<Texture> greenConstant = std::make_shared<ConstantTexture>(Vec3f(0.14f, 0.45f, 0.091f));
	std::shared_ptr<Texture> whiteConstant = std::make_shared<ConstantTexture>(Vec3f(0.725f, 0.71f, 0.68f));

	std::shared_ptr<Material> red = std::make_shared<Lambertian>(redConstant);
	std::shared_ptr<Material> green = std::make_shared<Lambertian>(greenConstant);
	std::shared_ptr<Material> white = std::make_shared<Lambertian>(whiteConstant);
	std::shared_ptr<Material> light = std::make_shared<DiffuseLight>(Vec3f(47.8348f, 38.5664f, 31.0808f));

	std::vector<Object*> objectList;
	objectList.push_back(new Mesh("res/cornellbox/floor.obj", white));
	objectList.push_back(new Mesh("res/cornellbox/shortbox.obj", white));
	objectList.push_back(new Mesh("res/cornellbox/tallbox.obj", white));
	objectList.push_back(new Mesh("res/cornellbox/left.obj", red));
	objectList.push_back(new Mesh("res/cornellbox/right.obj", green));
	objectList.push_back(new Mesh("res/cornellbox/light.obj", light));

	scene.setObjectList(std::move(objectList));

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
		int R = clamp<float>(0.0f, 1.0f, std::powf(color[0], gamma)) * 255.0f;
		int G = clamp<float>(0.0f, 1.0f, std::powf(color[1], gamma)) * 255.0f;
		int B = clamp<float>(0.0f, 1.0f, std::powf(color[2], gamma)) * 255.0f;
		fprintf(fp, "\n%d %d %d", R, G, B);
	}
	fclose(fp);
}