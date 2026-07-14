#ifndef SYSTEMS_H_
#define SYSTEMS_H_


//Чтобы устранить циклическую зависимость между MovementSystem и Scene, используется объявление класса Scene в .h файле,
//а подключение scene.h происходит в .cpp файле

class Scene;

class MovementSystem {
public:
	void Update(float dt, Scene& scene);
};


#endif //SYSTEMS_H_
