#pragma once
#include "Direction.h"
#include "ProgramExceptions.h"
class Player
{
public:
	Player(int x, int y, Direction dir) : x(x), y(y), dir(dir), isAlive(true) { }

	void Move() {
		int vectorX = 0, vectorY = 0;
		switch (dir) {
		case UP:
			vectorX = -1;
			break;
		case DOWN:
			vectorX = 1;
			break;
		case RIGHT:
			vectorY = 1;
			break;
		case LEFT:
			vectorY = -1;
			break;
		}

		x += vectorX;
		y += vectorY;
	}
	//0-null, 1-tank, 2-bullet, 3-wall
	bool CheckCollision(char** field){
		if (field[x][y] == ' ') return 0;
		else if (field[x][y] == '^'
			|| field[x][y] == 'v'
			|| field[x][y] == '>'
			|| field[x][y] == '<') return 1;
		else if (field[x][y] == '-' || field[x][y] == '|') return 2;
		else if (field[x][y] == '#') return 3;
	}
	void MoveBackward() {
		int vectorX = 0, vectorY = 0;
		switch (dir) {
		case UP:
			vectorX = -1;
			break;
		case DOWN:
			vectorX = 1;
			break;
		case RIGHT:
			vectorY = 1;
			break;
		case LEFT:
			vectorY = -1;
			break;
		}

		x += -vectorX;
		y += -vectorY;
	}

	void SetDirection(Direction dir) {
		this->dir = dir;
	}

	int GetX() const {
		return x;
	}
	int GetY() const {
		return y;
	}
	Direction GetDir() const {
		return dir;
	}
	bool CheckIsAlive() const {
		return isAlive;
	}
	void Died() {
		isAlive = false;
	}

private:
	int x, y;
	Direction dir;
	bool isAlive;
};