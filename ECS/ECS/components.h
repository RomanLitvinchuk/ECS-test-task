#ifndef COMPONENTS_H_
#define COMPONENTS_H_
#include <vector>

//Компоненты наследуются от базового класса Component. Виртуальный деструктор нужен, чтобы объекты дочерних классов
//корректно удалялись из памяти, когда уничтожаются через указатель на базовый класс Component

class Component {
public:
	virtual ~Component() = default;
};

class HealthComponent : public Component {
public:
	int max_;
	int current_;

	HealthComponent(int max, int current) : max_(max), current_(current) {}
};

//Вот здесь вместо std::vector<float> у меня была мысль использовать Vector3 из библиотеки SimpleMath для DirectX, но тогда нужно подключать эти сторонние библиотеки
//Я не был уверен, насколько это целесообразно в данном случае, поэтому оставил так
class TransformComponent : public Component {
public:
	std::vector<float> position_;
	std::vector<float> rotation_;
	std::vector<float> scale_;

	TransformComponent(std::vector<float> pos, std::vector<float> rot, std::vector<float> scale) :
		position_(pos), rotation_(rot), scale_(scale) {}
};


#endif //COMPONENTS_H_
