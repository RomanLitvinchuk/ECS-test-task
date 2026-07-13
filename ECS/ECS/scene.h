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
using Entity = uint32_t;

struct EntityRecord {
	Entity id_;
	std::vector<std::unique_ptr<Component>> components;
};

class Scene {
private:
	std::vector<EntityRecord> registry;
	MovementSystem movement_;
public:
	void CreateEntity(Entity entity);
	void DestroyEntity(Entity entity);

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
	void Load();

	void Initialize();

	void Display();
};


#endif //SCENE_H_
