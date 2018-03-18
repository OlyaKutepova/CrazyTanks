#pragma once
#include <random>
#include "Direction.h"


class Enemy
{
public:
	Enemy(char** mp) : x(0), y(0) {
		int lastX = 0, lastY = 0;

		for (int i = 0; i < 10; i++) 
		{
			for (int j = 0; j < 10; j++) 
			{
				if (mp[i][j] == '^' && (i != 8 && j != 5))
				{ 
					lastX = i;
					lastY = j;
				}
			}
		}

		if (lastX != 0 && lastY != 0) {
			lastY += (2 + rand()  % 3);
			if (lastY >= 9) {
				lastY = 0 + rand() % 3;
				lastX += (2 + rand() % 3);
				if (lastX >= 9) throw "troubled borders";
			}
			x = lastX;
			y = lastY;
			vectorX = -1;
			vectorY = 0;
			dir = UP;
			mp[x][y] = '^';
			isExist = true;
		}
		else{
			lastX = 1 + rand() % 2;
			lastY = 1 + rand() % 3;
			x = lastX;
			y = lastY;
			vectorX = -1;
			vectorY = 0;
			dir = UP;
			mp[x][y] = '^';
			isExist = true;
		}
	}

	Direction RandomizeDirection() {
		int choose = 1 + rand() % 4;
		switch (choose)
		{
		case 1:
			ChangeVector(1, 0);
			return DOWN;
			break;
		case 2:
			ChangeVector(-1, 0);
			return UP;
			break;
		case 3:
			ChangeVector(0, 1);
			return RIGHT;
			break;
		case 4:
			ChangeVector(0, -1);
			return LEFT;
			break;
		default:
			throw 2;
			break;
		}
	}
	void MoveTankRnd() {
		Direction moving = RandomizeDirection();
		x += vectorX;
		y += vectorY;
		dir = moving;	
	}
	void MoveTankFact(int vectorX = 0, int vectorY = 1) {
		x += vectorX;
		y += vectorY;
		char EnemyTank = ' ';
		/* !!!!  Написать проверку, куда смотрит танк после движения. Пример: х=1, у=0, значит dir = DOWN*/

		if (vectorX == 1 && vectorY == 0) {
			EnemyTank = 'v';
		}
		if (vectorX == -1 && vectorY == 0) {
			EnemyTank = '^';
		}
		if (vectorX == 0 && vectorY == 1) {
			EnemyTank = '>';
		}
		if (vectorX == 0 && vectorY == -1) {
			EnemyTank = '<';
		}

	}
	bool CheckCollision(char**  field) {
		if (field[x][y] == ' ') return false;
		else return true;
	}
	bool CheckExist() {
		return isExist;
	}
	bool InvertExist() { //Инвертирует состояние танка
		isExist = -isExist;
		return isExist;
	}
	int GetX() const { return x; }
	int GetY() const { return y; }
	Direction GetDir() const { return dir; }
	int GetVector(char coord) const {
		if (coord == 'x') return vectorX;
		if (coord == 'y') return vectorY;
		return 100;
	}

private:
	Direction dir;
	int x, y;
	int vectorX, vectorY;
	bool isExist;
	void ChangeVector(int x, int y) {
		vectorX = x;
		vectorY = y;
	}
};