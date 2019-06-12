#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include <vector>
#include "mygame.h"
#include "Enemy.h"
#include<iostream>

namespace game_framework {
	Enemy::Enemy(int x,int y,int dir):index_x(x),index_y(y),direction(dir)
	{
		Initialize();
	}
	Enemy::~Enemy() { TRACE("~Enemy()\n"); }
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
	void Enemy::Hurt(int damage)
	{
		lifePoint -= damage;
	}
	int Enemy::GetLifePoint()
	{
		return lifePoint;
	}
}