#include <iostream>
#include "scene.h"
#include "systems.h"

int main()
{
	Scene scene;

	int choice;
	std::cout << "Do you want to load save file? 0 - Yes 1 - No" << std::endl;
	std::cin >> choice;

	switch (choice) {
	case 0:
		scene.Load();
		break;

	case 1:
		scene.Initialize();
		break;

	default:
		std::cout << "Invalid input" << std::endl;
		break;
	}
}

