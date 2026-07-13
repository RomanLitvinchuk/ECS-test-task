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
	for (auto it = registry.begin(); it != registry.end(); ++it){
		if (it->id_ == entity) {
			registry.erase(it);
			std::cout << "Entity " << entity << " was destroyed" << std::endl;
			return;
		}
	}
	std::stringstream ss;
	ss << "Warning: Entity " << entity << " wasn't found" << "\n";
	OutputDebugStringA(ss.str().c_str());
}

void Scene::Update(float dt)
{
	movement_.Update(dt, *this);
}


void Scene::Save() {
	json saveFile;
	saveFile.clear();
	for (const auto& reg : registry) {
		json entityJson;
		auto id = reg.id_;
		entityJson["id"] = id;
		entityJson["components"] = json::object();
		if (auto health = GetComponent<HealthComponent>(id)) {
			entityJson["components"]["HealthComponent"]["max"] = health->max_;
			entityJson["components"]["HealthComponent"]["current"] = health->current_;
		}
		if (auto transform = GetComponent<TransformComponent>(id)) {
			entityJson["components"]["TransformComponent"]["position"] = transform->position_;
			entityJson["components"]["TransformComponent"]["rotation"] = transform->rotation_;
			entityJson["components"]["TransformComponent"]["scale"] = transform->scale_;
		}
		saveFile["entities"].push_back(entityJson);
	}
	std::ofstream outputFile("saveFile.json");
	outputFile << saveFile.dump(4);
}

void Scene::Load() {
	std::ifstream file("saveFile.json");
	if (!file.good()) {
		std::cout << "File doesn't exist or it's not readable" << std::endl;
		return;
	}
	registry.clear();
	json jFile = json::parse(file);
	for (auto& entityJson : jFile["entities"]) {
		auto id = entityJson["id"].get<uint32_t>();
		CreateEntity(id);
		if (entityJson["components"].contains("HealthComponent")) {
			int max = entityJson["components"]["HealthComponent"]["max"].get<int>();
			int current = entityJson["components"]["HealthComponent"]["current"].get<int>();
			AddComponent<HealthComponent>(id, max, current);
		}
		if (entityJson["components"].contains("TransformComponent")) {
			auto pos = entityJson["components"]["TransformComponent"]["position"].get<std::vector<float>>();
			auto rot = entityJson["components"]["TransformComponent"]["rotation"].get<std::vector<float>>();
			auto scale = entityJson["components"]["TransformComponent"]["scale"].get<std::vector<float>>();
			AddComponent<TransformComponent>(id, pos, rot, scale);
		}
	}
	std::cout << "Save file was loaded" << std::endl;
	Display();
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

	Display();

	Update(1.0f);
	Update(1.0f);
	Update(0.5f);

	DestroyEntity(3);
	DestroyEntity(0);

	Display();

	Save();
}

void Scene::Display()
{
	std::cout << "====================================" << std::endl;
	for (auto& entity : registry) {
		auto id = entity.id_;
		std::cout << "\nID: " << id << "\n";
		if (auto health = GetComponent<HealthComponent>(id)) {
			std::cout << "\tHealth Component" << std::endl;
			std::cout << "\t\tMax: " << health->max_ << " Current: " << health->current_ << "\n";
		}
		if (auto transform = GetComponent<TransformComponent>(id)) {
			std::cout << "\tTransform Component" << std::endl;
			std::cout << "\t\tPos: " << transform->position_[0] << " " << transform->position_[1] << " " << transform->position_[2] << "\n";
			std::cout << "\t\tRot: " << transform->rotation_[0] << " " << transform->rotation_[1] << " " << transform->rotation_[2] << "\n";
			std::cout << "\t\tScl: " << transform->scale_[0] << " " << transform->scale_[1] << " " << transform->scale_[2] << "\n";
		}
	}
	std::cout << "====================================" << std::endl;
	std::cout << "\n";
}
