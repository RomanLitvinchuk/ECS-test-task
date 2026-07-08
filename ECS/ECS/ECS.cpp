#include <iostream>
#include "scene.h"
#include "systems.h"

int main()
{
	Scene scene;
	scene.CreateEntity();
	scene.CreateEntity();
	scene.CreateEntity();

	scene.AddComponent<HealthComponent>(1, 100, 50);
	scene.AddComponent<TransformComponent>(0, std::vector<float>{ 0.0f, 0.0f, 0.0f }, std::vector<float>{ 0.0f, 0.0f, 0.0f }, std::vector<float>{ 1.0f, 1.0f, 1.0f });
	scene.AddComponent<HealthComponent>(2, 50, 50);
	scene.AddComponent<TransformComponent>(2, std::vector<float>{ 10.0f, 10.0f, 5.0f }, std::vector<float>{ 0.0f, 0.0f, 0.0f }, std::vector<float>{ 1.0f, 1.0f, 1.0f });

	scene.Update(1.0f);
	scene.Update(1.0f);
	scene.Update(0.5f);

	scene.DestroyEntity(3);
	scene.DestroyEntity(0);
}

