#include "scene.h"
void Scene::CreateEntity()
{
	Entity id;
	id = nextId_++;
	registry.push_back({ id, {} });
	std::cout << "Create entity. ID: " << id << std::endl;
}

void Scene::DestroyEntity(Entity entity)
{
	for (auto& record : registry) {
		if (record.id_ == entity) {
			record.components.clear();
			std::cout << "Entity " << record.id_ << " was destroyed";
			return;
		}
	}
	std::cout << "Entity " << entity << " isn't found" << std::endl;
}

void Scene::Update(float dt)
{
	movement_.Update(dt, *this);
}
