#pragma once
#include "Direction.h"
#include "ProgramExceptions.h"

class Bullet
{
public:
	Bullet(int x, int y, Direction dir, bool isPlayer) : x(x), y(y), dir(dir), isPlayer(isPlayer) {
		if (x >= 9 || y >= 9 || x <= 0 || y <= 0) throw WrongBulletStartPosition();
	}

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
	//0 - null, 1 - enemy tank, 2 - player's tank, 3 - wall
	int CheckCollision(char** field, int PLx, int PLy) {
		if (field[x][y] == ' ') return 0;
		else if (this->x == PLx && this->y == PLy) return 2;
		else if (field[x][y] == '^'
			|| field[x][y] == 'v'
			|| field[x][y] == '>'
			|| field[x][y] == '<') return 1;
		else if (field[x][y] == '#') return 3;
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
	bool GetIsPlayer() const {
		return isPlayer;
	}

private:
	int x, y;
	Direction dir;
	bool isPlayer;
};

