#pragma once
#include "Enemy.h"
#include "Player.h"
#include "Bullet.h"
#include <iostream>
#include <vector>

const int MAX_ENEMIES = 5;
const int MAX_BULLETS = MAX_ENEMIES + 1;
const int CHANCE_SHOOT = 40; //%

class Console
{
public:
	Console() {
		for (int i = 0; i < MAX_BULLETS; i++) {
			bullets[i] = nullptr;
		}
		BulOnScene = 0;
		player = new Player(8, 5, UP);

		map = new char*[10];
		for (int i = 0; i < 10; i++) {
			map[i] = new char[10];
		}
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
			{
				if (i == 0 || j == 0 || i == 9 || j == 9) map[i][j] = '#';
				else map[i][j] = ' ';
			}


		InitializeEnemies();
	}
	void InitializeEnemies() {
		enemy_map = new Enemy*[MAX_ENEMIES];

		for (int i = 0; i < MAX_ENEMIES; i++) {
			enemy_map[i] = new Enemy(map);
		}
	}
	void RefreshMap() {
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
			{
				map[i][j] = ' ';
			}

		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
			{
				if (i == 0 || j == 0 || i == 9 || j == 9) map[i][j] = '#';
			}

		int masX[MAX_ENEMIES];
		int masY[MAX_ENEMIES];
		Direction masDir[MAX_ENEMIES];

		for (int i = 0; i < MAX_ENEMIES; i++) {
			masX[i] = -10;
			masY[i] = -10;
			masDir[i] = UP;
		}

		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy_map[i]->CheckExist()) {
				masX[i] = enemy_map[i]->GetX();
				masY[i] = enemy_map[i]->GetY();
				masDir[i] = enemy_map[i]->GetDir();
			}
			else continue;
		}

		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (masX[i] != -10) {
				char symbol = ' ';
				switch (masDir[i]) {
				case UP:
					symbol = '^';
					break;
				case DOWN:
					symbol = 'v';
					break;
				case RIGHT:
					symbol = '>';
					break;
				case LEFT:
					symbol = '<';
					break;
				}

				map[masX[i]][masY[i]] = symbol;
			}
		}


		int masBulX[MAX_BULLETS];
		int masBulY[MAX_BULLETS];
		Direction masBulDir[MAX_BULLETS];

		for (int i = 0; i < MAX_BULLETS; i++) 
		{
			masBulX[i] = -10;
			masBulY[i] = -10;
			masBulDir[i] = UP;
		}

		for (int i = 0; i < MAX_BULLETS; i++)
		{
			if (bullets[i] != nullptr)
			{
				masBulX[i] = bullets[i]->GetX();
				masBulY[i] = bullets[i]->GetY();
				masBulDir[i] = bullets[i]->GetDir();
			}
			else continue;

		}
		for (int i = 0; i < MAX_BULLETS; i++) {
			if (masBulX[i] != -10) {
				char symbol = ' ';
				switch (masBulDir[i]) {
				case UP:
					symbol = '|';
					break;
				case DOWN:
					symbol = '|';
					break;
				case RIGHT:
					symbol = '-';
					break;
				case LEFT:
					symbol = '-';
					break;
				}

				map[masBulX[i]][masBulY[i]] = symbol;
			}
		}


		char symbol = ' ';
		switch (player->GetDir()) {
		case UP:
			symbol = '^';
			break;
		case DOWN:
			symbol = 'v';
			break;
		case RIGHT:
			symbol = '>';
			break;
		case LEFT:
			symbol = '<';
			break;
		}
		map[player->GetX()][player->GetY()] = symbol;
	}
	Bullet* CreateBullet(bool isPlayer, int x, int y, Direction dir) {

		if (BulOnScene == MAX_BULLETS) return nullptr;
		Bullet* bul = nullptr;
		try {
			bul = new Bullet(x, y, dir, isPlayer);
		}
		catch (WrongBulletStartPosition) {
			return nullptr;
		}
		int i = 0;
		for (; i < MAX_BULLETS; i++) {
			if (bullets[i] == nullptr) {
				bullets[i] = bul;
				BulOnScene++;
				break;
			}
		}
		if (i == MAX_BULLETS) throw TooMuchBulletsException();
		return bul;
	}
	bool TryToShoot(const Enemy* const enemy) {
		int tik = rand() + 0 % 100;
		if (tik >= CHANCE_SHOOT) {
			int x = enemy->GetX();
			int y = enemy->GetY();
			Direction dir = enemy->GetDir();
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
			CreateBullet(false, x, y, dir);
		}
		else return false;
	}
	void MoveRandAllEnemies(char**  map) {
		for (int i = 0; i < MAX_ENEMIES; i++)
		{
			if (enemy_map[i]->CheckExist()) {
				enemy_map[i]->MoveTankRnd();
				if (enemy_map[i]->CheckCollision(map))
					enemy_map[i]->MoveTankFact(
						-(enemy_map[i]->GetVector('x')),
						-(enemy_map[i]->GetVector('y')));
			}
		}
	}
	Enemy** GetEnemies() {
		return enemy_map;
	}
	char** GetMap() {
		return map;
	}
	Player* GetPlayer() {
		return player;
	}
	Bullet** GetBullets() {
		return bullets;
	}
	int GetMaxEnemies() const {
		return MAX_ENEMIES;
	}
	int GetMaxBullets() const {
		return MAX_BULLETS;
	}
	void MovePlayer(Direction dir) {
		player->SetDirection(dir);
		player->Move();
		switch (player->CheckCollision(map)) {
		case 0:
			break;
		case 1:
			player->MoveBackward();
			break;
		case 2:
			player->Died();
			throw PlayerKilledException();
			break;
		case 3:
			player->MoveBackward();
			break;
		}
	}
	void TryToShootAllEnemies() {
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy_map[i]->CheckExist()) {
				TryToShoot(enemy_map[i]);
			}
		}
	}
	void DeleteBullet(int x, int y) {
		int pos = 0;
		for (; pos < MAX_BULLETS; pos++) {
			if (bullets[pos] != nullptr) {
				if (bullets[pos]->GetX() == x && bullets[pos]->GetY() == y) break;
			}
		}
		if (pos == MAX_BULLETS) return;
		delete bullets[pos];
		bullets[pos] = nullptr;
	}
	void TankKilled(int x, int y) {
		int pos = 0;
		for (; pos < MAX_ENEMIES; pos++) {
			if (enemy_map[pos]->CheckExist()) {
				if (enemy_map[pos]->GetX() == x && enemy_map[pos]->GetY() == y) break;
			}
		}

		if (pos == MAX_ENEMIES) return;
		enemy_map[pos]->InvertExist();
	}
	void ReduseBullet() {
		BulOnScene--;
	}

private:
	char** map;
	int BulOnScene;
	Enemy** enemy_map;// массив противников
	Player* player;
	Bullet* bullets[MAX_BULLETS];
};