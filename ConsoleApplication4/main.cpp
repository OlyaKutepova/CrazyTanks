// ConsoleApplication4.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <time.h>
#include "iostream" 
#include "Console.h"
#include "Windows.h"
#include <conio.h>
#include "MapView.h"


using namespace std;

int main()
{
	srand(time(NULL));
	bool gamerunning = true; 
	Console console;
	MapView mapView(console.GetMap());
	Bullet** bull = console.GetBullets();
	bool PlayerMoved = false;
	while (gamerunning){
		
		console.RefreshMap();
		mapView.Draw(); 

		
		int key = _getch();
		if (!_kbhit()) {
			PlayerMoved = true;
			switch (_getch()) {
			case 65: //up
				console.GetPlayer()->SetDirection(UP);
			case 66: //down
				console.GetPlayer()->SetDirection(DOWN);
				break;
			case 67: //right
				console.GetPlayer()->SetDirection(RIGHT);
				break;
			case 68: //left
				console.GetPlayer()->SetDirection(LEFT);
				break;
			case 32:
			{
				int hx, hy;
				Direction hdir;
				hdir = console.GetPlayer()->GetDir();
				hx = console.GetPlayer()->GetX();
				hy = console.GetPlayer()->GetY();

				int hvectorX = 0, hvectorY = 0;

				switch (hdir) {
				case UP:
					hvectorX = -1;
					break;
				case DOWN:
					hvectorX = 1;
					break;
				case RIGHT:
					hvectorY = 1;
					break;
				case LEFT:
					hvectorY = -1;
					break;
				}

				hx += hvectorX;
				hy += hvectorY;

				console.CreateBullet(true, hx, hy, hdir);
			}
			break;
			}
		}
		else PlayerMoved = false;
		console.MoveRandAllEnemies(console.GetMap());
		console.TryToShootAllEnemies();
		try {
			for (int i = 0; i < MAX_BULLETS; i++) {
				if (bull[i] != nullptr) {
					bull[i]->Move();
					int col = bull[i]->CheckCollision(console.GetMap(), console.GetPlayer()->GetX(), console.GetPlayer()->GetY());
					switch (col) {
					case 0: break;
					case 1:
						if (bull[i]->GetIsPlayer()) {
							console.TankKilled(bull[i]->GetX(), bull[i]->GetY());
							bull[i] = nullptr;
							console.ReduseBullet();
						}
						else {
							bull[i] = nullptr;
							console.ReduseBullet();
						}
						break;
					case 2:
						if (bull[i]->GetIsPlayer()) {
							bull[i] = nullptr;
							console.ReduseBullet();
						}
						else {
							bull[i] = nullptr;
							console.ReduseBullet();
							console.GetPlayer()->Died();
						}
						break;
					case 3:
						bull[i] = nullptr;
						console.ReduseBullet();
						break;
					}
				}
			}
		}
		catch (PlayerKilledException) {
			gamerunning = false;
			break;
		}
		if (PlayerMoved) {
			console.GetPlayer()->Move();
			switch (console.GetPlayer()->CheckCollision(console.GetMap())) {
			case 0: break;
			case 1: console.GetPlayer()->MoveBackward(); break;
			case 3: console.GetPlayer()->MoveBackward(); break;
			}
		}

		Sleep(0.5f);
	}
	system("pause");
}

