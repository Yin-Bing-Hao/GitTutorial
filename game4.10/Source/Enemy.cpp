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
		test.AddBitmap("Bitmaps/Soldier2_LD.bmp", RGB(255, 255, 255));
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
			test.SetTopLeft(x, y);
			test.OnShow();
		}
	}
}