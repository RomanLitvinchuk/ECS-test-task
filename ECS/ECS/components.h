#ifndef COMPONENTS_H_
#define COMPONENTS_H_
#include <vector>

class Component {
public:
	virtual ~Component() = default;
};

class HealthComponent : public Component {
public:
	int max_;
	int current_;

	HealthComponent() : max_(0), current_(0) {}
	HealthComponent(int max, int current) : max_(max), current_(current) {}
};

//struct Position {
//	float x;
//	float y;
//	float z;
//};
//
//struct Rotation {
//	float x;
//	float y;
//	float z;
//};
//
//struct Scale {
//	float x;
//	float y;
//	float z;
//};

class TransformComponent : public Component {
public:
	std::vector<float> position_;
	std::vector<float> rotation_;
	std::vector<float> scale_;

	TransformComponent(std::vector<float> pos, std::vector<float> rot, std::vector<float> scale) :
		position_(pos), rotation_(rot), scale_(scale) {}
};


#endif //COMPONENTS_H_
