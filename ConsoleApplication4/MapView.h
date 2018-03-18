#pragma once
#include <iostream>

class MapView
{
public:
	MapView(char** map) : map(map){}

	void Draw() const { 
		system("cls");

		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++)
				std::cout << map[i][j];
			std::cout << std::endl;
		} 
	} 
private:
	char** map;
};