#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include <vector>
#include <cmath>
#include <thread>
#include <memory>
#include "mygame.h"
#include "Weapons.h"
#include "Soldier.h"
#include "Enemy.h"
#include<iostream>
namespace game_framework {
	Enemy::Enemy(int x,int y,int dir):index_x(x),index_y(y), init_way(dir)
	{
		shoot_count_time = 0;
		reaction_time = 20;
		lock_enemy = -1;
		Initialize();
	}
	Enemy::~Enemy() { 
		TRACE("~Enemy()\n"); 
		delete weapon;
	}
	void Enemy::Initialize()
	{
		const int X_POS = SIZE;
		const int Y_POS = SIZE;
		isWatchDown = isWatchLeft = isWatchLeftDown = isWatchLeftUp = isWatchRight = isWatchRightDown = isWatchRightUp = isWatchUp = false;
		isMovingLeft = isMovingRight = isMovingUp = isMovingDown = isMovingLeftDown = isMovingLeftUp = isMovingRightDown = isMovingRightUp = false;
		isSaw = false;
		x = index_x * 40;
		y = index_y * 40;
		lifePoint = 100;
		weapon = new P9();
		target = NULL;
	}
	void Enemy::LoadBitmap()
	{
		enemy_U.LoadBitmap("Bitmaps/enemy_Up.bmp", RGB(255, 255, 255));
		enemy_RU.LoadBitmap("Bitmaps/enemy_RU.bmp", RGB(255, 255, 255));
		enemy_R.LoadBitmap("Bitmaps/enemy_Right.bmp", RGB(255, 255, 255));
		enemy_RD.LoadBitmap("Bitmaps/enemy_RD.bmp", RGB(255, 255, 255));
		enemy_D.LoadBitmap("Bitmaps/enemy_Down.bmp", RGB(255, 255, 255));
		enemy_LD.LoadBitmap("Bitmaps/enemy_LD.bmp", RGB(255, 255, 255));
		enemy_L.LoadBitmap("Bitmaps/enemy_Left.bmp", RGB(255, 255, 255));
		enemy_LU.LoadBitmap("Bitmaps/enemy_LU.bmp", RGB(255, 255, 255));
	}
	int Enemy::GetIndexX()
	{
		return index_x;
	}
	int Enemy::GetIndexY()
	{
		return index_y;
	}
	void Enemy::SetIsSaw(bool flag)
	{
		isSaw = flag;
	}
	void Enemy::OnMove()
	{
		//TRACE("enemy:%d %d\n", x, y);
	}
	void Enemy::OnShow()
	{
		if (isSaw)
		{
			if (lock_enemy != -1)direction = lock_enemy;
			else direction = init_way;
			switch (direction) {
			case 0:
				enemy_U.SetTopLeft(x, y);
				enemy_U.ShowBitmap();
				break;

			case 1:
				enemy_RU.SetTopLeft(x, y);
				enemy_RU.ShowBitmap();
				break;

			case 2:
				enemy_R.SetTopLeft(x, y);
				enemy_R.ShowBitmap();
				break;

			case 3:
				enemy_RD.SetTopLeft(x, y);
				enemy_RD.ShowBitmap();
				break;

			case 4:
				enemy_D.SetTopLeft(x, y);
				enemy_D.ShowBitmap();
				break;

			case 5:
				enemy_LD.SetTopLeft(x, y);
				enemy_LD.ShowBitmap();
				break;

			case 6:
				enemy_L.SetTopLeft(x, y);
				enemy_L.ShowBitmap();
				break;

			case 7:
				enemy_LU.SetTopLeft(x, y);
				enemy_LU.ShowBitmap();
				break;

			default:
				break;
			}
		}
	}
	void Enemy::GetHurt(int damage)
	{
		lifePoint -= damage;
	}
	int Enemy::GetLifePoint()
	{
		return lifePoint;
	}
	void Enemy::searchEnemy(CGameMap* map, vector<Soldier*>& players)
	{
		double dx, dy;
		double Lx, Ly;
		double rotate_start, rotate_end;
		int Light_ix, Light_iy, Lix, Liy;
		double pi;
		double enemy_dir = -1;
		Soldier *_target = NULL;

		switch (direction)
		{
		case 0:
			rotate_start = 45;
			break;

		case 1:
			rotate_start = 0;
			break;

		case 2:
			rotate_start = 315;
			break;

		case 3:
			rotate_start = 270;
			break;

		case 4:
			rotate_start = 225;
			break;

		case 5:
			rotate_start = 180;
			break;

		case 6:
			rotate_start = 135;
			break;

		case 7:
			rotate_start = 90;
			break;
		}
		pi = 3.14159265 / 180.0;
		rotate_end = rotate_start + 90;
		TRACE("enemy search:%f %f", rotate_start, rotate_end);
		//TRACE("HEHEHEHEHEHE\n");
		for (double rotate = rotate_start; rotate <= rotate_end; rotate += 0.125)
		{
			Lx = x + 20;
			Ly = y + 20;
			Lix = static_cast<int>(Lx) / 40;
			Liy = static_cast<int>(Ly) / 40;

			dx = 20 * cos(rotate * pi);
			dy = -20 * sin(rotate * pi);
			//------µø³¥
			Light_ix = static_cast<int>(Lx) / 20;
			Light_iy = static_cast<int>(Ly) / 20;

			//TRACE("Search:%d %d %d\n", dx, dy, rotate);

			do
			{
				//TRACE("Search:%d %d %d\n", Lix, Liy, rotate);
				Lix = static_cast<int>(Lx) / 40;
				Liy = static_cast<int>(Ly) / 40;
				Light_ix = static_cast<int>(Lx) / 20;
				Light_iy = static_cast<int>(Ly) / 20;
				if (map->GetIndexValue(Lix, Liy) == 1)
				{
					for (vector<Soldier*>::iterator iter = players.begin(); iter != players.end(); iter++)
					{
						if ((*iter)->GetIndexX() == Lix && (*iter)->GetIndexY() == Liy)
						{
							enemy_dir = rotate;
							_target = (*iter);
							break;
						}
					}
				}
				Lx += dx;
				Ly += dy;
			} while (map->GetIndexValue(Lix, Liy) < 5 && (Lx > 0 && Lx < ROW*SIZE&&Ly>0 && Ly < COL*SIZE));
			if (_target != NULL)
			{
				
				break;
			}
		}
		target = _target;
		if (enemy_dir != -1)
		{
			if (enemy_dir >= 360)enemy_dir -= 360;
			if (enemy_dir <= 22.5 || enemy_dir > 337.5)lock_enemy = 2;
			else if (enemy_dir <= 67.5 && enemy_dir > 22.5)lock_enemy = 1;
			else if (enemy_dir <= 112.5 && enemy_dir > 67.5)lock_enemy = 0;
			else if (enemy_dir <= 157.5 && enemy_dir > 112.5)lock_enemy = 7;
			else if (enemy_dir <= 202.5 && enemy_dir > 157.5)lock_enemy = 6;
			else if (enemy_dir <= 247.5 && enemy_dir > 202.5)lock_enemy = 5;
			else if (enemy_dir <= 292.5 && enemy_dir > 247.5)lock_enemy = 4;
			else if (enemy_dir <= 337.5 && enemy_dir > 292.5)lock_enemy = 3;
		}
		else lock_enemy = -1;
		//lock_enemy = enemy_dir;
	}

	void Enemy::attackPlayer()
	{
		TRACE("WTFFFFFF\n");
		if (target != NULL)
		{
			
			if (shoot_count_time > reaction_time)
			{
				//TRACE("SHOOT!!!\n");

				this->shoot();
				shoot_count_time = 5;
			}
			shoot_count_time++;
		}
		else
		{
			shoot_count_time = 0;
		}
	}
	void Enemy::shoot()
	{
		int damage = weapon->GetDamage();
		TRACE("enemy shoot\n");
		unique_ptr<thread> fire(new thread(&Weapon::Fire, weapon));
		target->GetHurt(damage);
		if (target->GetLifePoint() <= 0) {
			target = NULL;
		}
		if (fire->joinable())fire->join();

	}
}

