#include "scene.h"
#include <fstream>

void Scene::CreateEntity(Entity entity)
{
	registry.push_back({ entity, {} });
	std::cout << "Create entity. ID: " << entity << std::endl;
}

void Scene::DestroyEntity(Entity entity)
{
	int k = 0;
	for (auto& record : registry) {
		if (record.id_ == entity) {
			record.components.clear();
			auto it = registry.begin() + k;
			registry.erase(it);
			std::cout << "Entity " << entity << " was destroyed" << std::endl;
			return;
		}
		k++;
	}
	std::cout << "Entity " << entity << " isn't found" << std::endl;
}

void Scene::Update(float dt)
{
	movement_.Update(dt, *this);
}


void Scene::Save() {
	saveFile_.clear();
	for (const auto& reg : registry) {
		json entityJson;
		entityJson["id"] = reg.id_;
		entityJson["components"] = json::object();
		if (auto health = GetComponent<HealthComponent>(reg.id_)) {
			entityJson["components"]["HealthComponent"]["max"] = health->max_;
			entityJson["components"]["HealthComponent"]["current"] = health->current_;
		}
		if (auto transform = GetComponent<TransformComponent>(reg.id_)) {
			entityJson["components"]["TransformComponent"]["position"] = transform->position_;
			entityJson["components"]["TransformComponent"]["rotation"] = transform->rotation_;
			entityJson["components"]["TransformComponent"]["scale"] = transform->scale_;
		}
		saveFile_["entities"].push_back(entityJson);
	}
	std::ofstream outputFile("saveFile.json");
	outputFile << saveFile_.dump(4);
}

void Scene::Load() {
	std::ifstream file("saveFile.json");
	if (!file.good()) {
		std::cout << "File doesn't exist or it's not readable" << std::endl;
		return;
	}
	json jFile = json::parse(file);
	for (auto& entityJson : jFile["entities"]) {
		auto id = entityJson["id"].get<uint32_t>();
		CreateEntity(id);
		if (entityJson.contains("HealthComponent")) {
			int max = entityJson["components"]["HealthComponent"]["max"].get<int>();
			int current = entityJson["components"]["HealthComponent"]["current"].get<int>();
			AddComponent<HealthComponent>(id, max, current);
		}
		if (entityJson.contains("TransformComponent")) {
			auto pos = entityJson["components"]["TransformComponent"]["position"].get<std::vector<float>>();
			auto rot = entityJson["components"]["TransformComponent"]["rotation"].get<std::vector<float>>();
			auto scale = entityJson["components"]["TransformComponent"]["scale"].get<std::vector<float>>();
			AddComponent<TransformComponent>(id, pos, rot, scale);
		}
	}
	std::cout << "Save file was loaded" << std::endl;
}

void Scene::Initialize()
{
	CreateEntity(0);
	CreateEntity(1);
	CreateEntity(2);

	AddComponent<HealthComponent>(1, 100, 50);
	AddComponent<TransformComponent>(0, std::vector<float>{ 0.0f, 0.0f, 0.0f }, std::vector<float>{ 0.0f, 0.0f, 0.0f }, std::vector<float>{ 1.0f, 1.0f, 1.0f });
	AddComponent<HealthComponent>(2, 50, 50);
	AddComponent<TransformComponent>(2, std::vector<float>{ 10.0f, 10.0f, 5.0f }, std::vector<float>{ 0.0f, 0.0f, 0.0f }, std::vector<float>{ 1.0f, 1.0f, 1.0f });

	Update(1.0f);
	Update(1.0f);
	Update(0.5f);

	DestroyEntity(3);
	DestroyEntity(0);

	Save();
}
