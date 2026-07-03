#include <iostream>
#include "scene.h"

int main()
{
	Scene scene;
	scene.CreateEntity();
	scene.CreateEntity();
	scene.AddComponent<HealthComponent>(1, 100, 50);
	scene.AddComponent<TransformComponent>(0, std::vector<float>{ 0.0f, 0.0f, 0.0f }, std::vector<float>{ 0.0f, 0.0f, 0.0f }, std::vector<float>{ 1.0f, 1.0f, 1.0f });

	scene.DestroyEntity(3);
	scene.DestroyEntity(0);

}

