#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include <memory>
#include <iostream>
#include <windows.h>
#include <sstream>
#include "components.h"
#include "systems.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
//Entity - это просто обёртка над uint32_t. При изучении ECS я видел различные вариации реализации Entity не только как обёртку,
//но и в виде класса, где помимо id был bool на то, является id активным или нет. Т.е. вместо удаления энтити из вектора
//с помощью итератора просто делать энтити неактивным.
using Entity = uint32_t;

struct EntityRecord {
	Entity id_;
	std::vector<std::unique_ptr<Component>> components;
};

//Класс-менеджер над ECS. 

class Scene {
private:
	std::vector<EntityRecord> registry;
	MovementSystem movement_;
	int nextId_ = 0;
	void CreateEntity(Entity entity);
public:
	Entity CreateEntity();
	void DestroyEntity(Entity entity);

	//Шаблонная функция для нахождения всех энтити с заданным компонентом. Применяется в системах.
	template <typename T>
	std::vector<Entity> Find() {
		std::vector<Entity> result;
		for (const auto& record : registry) {
			for (auto& comp : record.components) {
				if (dynamic_cast<T*>(comp.get())) {
					result.push_back(record.id_);
					break;
				}
			}
		}
		return result;
	}

	//Шаблонная функция добавления заданному энтити нового компонента. Благодаря Args можно передавать пакеты параметров,
	//далее использующиеся для подачи в конструктор заданного компонента
	template <typename T, typename... Args>
	void AddComponent(Entity entity, Args&&... args)
	{
		for (auto& record : registry) {
			if (record.id_ == entity) {
				record.components.push_back(std::make_unique<T>(std::forward<Args>(args)...));
				return;
			}
		}
		std::stringstream ss;
		ss << "Warning: Entity " << entity << " wasn't found" << "\n";
		OutputDebugStringA(ss.str().c_str());
	}
	
	//Шаблонная функция взятия компонента у энтити. Я отказался от дебага у этой функции в случае если не найден энтити по заданному id
	//или если у энтити нет заданного компонента, так как это достаточно частая ситуация и в таком случае просто возвращается nullptr
	template<typename T>
	T* GetComponent(Entity entity)
	{
		bool isFoundEntity = false;
		for (const auto& record : registry) {
			if (record.id_ == entity) {
				isFoundEntity = true;
				for (auto& comp : record.components) {
					if (T* casted = dynamic_cast<T*>(comp.get())) {
						return casted;
					}
				}
			}
		}
		return nullptr;
	}

	void Update(float dt);

	void Save();
	bool Load();

	void Initialize();

	void Display();
};


#endif //SCENE_H_
