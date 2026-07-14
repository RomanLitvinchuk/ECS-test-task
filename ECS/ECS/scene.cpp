#include "scene.h"
#include <fstream>

//Создание энтити. На вход подаётся id создаваемого энтити. Используется при загрузке информации об Entity из файла.
void Scene::CreateEntity(Entity entity)
{
	registry.push_back({ entity, {} });
	std::cout << "Create entity. ID: " << entity << std::endl;
}

//Создание энтити. Используется в обычных ситуациях. Сразу возвращает id энтити, чтобы воспользоваться им далее
//например при добавлении компонентов. До этого рассматривал вариант, когда CreateEntity ничего не возвращал,
//но подход с возвращением id показался мне более удобным, так как подавать в тот же AddComponent захардкоженный ID не очень удобно.
//Возможно, стоит рассмотреть вариант с использованием обоих подходов в виде 2 разных функций. 
//Одна возвращает ID, а вторая нет, и пользоваться той, что удобна в конкретный момент
Entity Scene::CreateEntity()
{
	Entity id;
	id = nextId_++;
	registry.push_back({ id, {} });
	std::cout << "Create entity. ID: " << id << std::endl;
	return id;
}

//Удаление энтити по итератору. На вход подаётся id энтити, который нужно удалить
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

//Отдельная функция обновления систем у класса Scene.
void Scene::Update(float dt)
{
	movement_.Update(dt, *this);
}

//Сохранение и загрузка с помощью nlohmann/json.
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

bool Scene::Load() {
	std::ifstream file("saveFile.json");
	if (!file.good()) {
		std::cout << "File doesn't exist or it's not readable" << std::endl;
		return false;
	}
	registry.clear();
	nextId_ = 0;
	Entity maxId = 0;
	json jFile = json::parse(file);
	for (auto& entityJson : jFile["entities"]) {
		auto id = entityJson["id"].get<uint32_t>();
		CreateEntity(id);
		if (id > maxId) maxId = id;
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
	//Увеличиваем счётчик ID на maxID + 1, чтобы в случае, если мы будем создавать энтити после загрузки из файла, 
	//то id новосозданных и загруженных из файла энтити не повторялись
	nextId_ = maxId + 1;
	std::cout << "Save file was loaded" << std::endl;
	Display();
	return true;
}

//Функция инициализации на случай, если пользователь выберет не загружать файл или файла не существует/повреждён.
void Scene::Initialize()
{
	Entity e0 = CreateEntity();
	Entity e1 = CreateEntity();
	Entity e2 = CreateEntity();

	AddComponent<HealthComponent>(e0, 100, 50);
	AddComponent<TransformComponent>(e1, std::vector<float>{ 0.0f, 0.0f, 0.0f }, std::vector<float>{ 0.0f, 0.0f, 0.0f }, std::vector<float>{ 1.0f, 1.0f, 1.0f });
	AddComponent<HealthComponent>(e2, 50, 50);
	AddComponent<TransformComponent>(e2, std::vector<float>{ 10.0f, 10.0f, 5.0f }, std::vector<float>{ 0.0f, 0.0f, 0.0f }, std::vector<float>{ 1.0f, 1.0f, 1.0f });

	Display();

	Update(1.0f);
	Update(1.0f);
	Update(0.5f);

	DestroyEntity(3);
	DestroyEntity(e0);

	Display();

	Save();
}

//Функция отображения информации об энтити и их компонентах
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
