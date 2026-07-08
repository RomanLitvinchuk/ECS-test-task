#include "systems.h"
#include "scene.h"

void MovementSystem::Update(float dt, Scene& scene)
{
	auto entities = scene.Find<TransformComponent>();
	for (auto& ent : entities) {
		auto comp = scene.GetComponent<TransformComponent>(ent);
		comp->position_[0] += 1.0f * dt;
		comp->position_[1] += 1.0f * dt;
		comp->position_[2] += 1.0f * dt;
		std::cout << "Entity " << ent << " moved to (" << comp->position_[0] << " " << comp->position_[1] << " " << comp->position_[2] << ")" << std::endl;
	}
}
