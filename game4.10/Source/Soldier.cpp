#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include <vector>
#include <cmath>
#include "mygame.h"
#include "Weapons.h"
#include "Enemy.h"
#include "Line.h"
#include "Soldier.h"
#include <thread>
#include <memory>
#include<iostream>
namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// Soldier: Eraser class
/////////////////////////////////////////////////////////////////////////////
#pragma region Basic
Soldier::Soldier(int x,int y,int z)
{
	//---初始化---//
	index_x = x;
	index_y = y;
	lifepoint = z;
	shoot_count_time = 0;
	reaction_time = 10;
	lock_enemy = -1;
	gun = 0;
	x1 = x2 = y1 = y2 = x11 = x12 = x21 = x22 = y11 = y12 = y21 = y22 = end_x = end_y = 0;
	isSetRoadLine = true;
	isChoosen = isWatchDown = isWatchLeft = isWatchLeftDown = isWatchLeftUp = isWatchRight = isWatchRightDown = isWatchRightUp = isWatchUp = isSetAction = false;
	isMovingLeft = isMovingRight = isMovingUp = isMovingDown = isMovingLeftDown = isMovingLeftUp = isMovingRightDown = isMovingRightUp = false;
	isMoveNextIndex = isInRoadLine = isShoot = false;
	way = roadLine.end();
	direction = 2;
    Initialize();	//圖片載入
}
Soldier::~Soldier()
{
	target = NULL;
	for (vector<Line*>::iterator iter = roadLine.begin();iter != roadLine.end();iter++)
	{
		delete (*iter);	//刪除現有路線
	}
	for (int i = 0;i < 2;i++)
	{
		delete equipment[i];	//刪除武器
	}
	roadLine.clear();
}
void Soldier::Initialize()
{
    const int X_POS = SIZE;
    const int Y_POS = SIZE;
    x = index_x* SIZE;
    y = index_y* SIZE;
	equipment[0] = new HK416();
	equipment[1] = new P9();
	weapon = equipment[0];
	target = NULL;
}
void Soldier::LoadBitmap()
{
	//---角色圖片載入--//
    peopleL.AddBitmap("Bitmaps/Soldier2_Left.bmp", RGB(255, 255, 255));
    peopleU.AddBitmap("Bitmaps/Soldier2_Up.bmp", RGB(255, 255, 255));
    peopleR.AddBitmap("Bitmaps/Soldier2_Right.bmp", RGB(255, 255, 255));
    peopleD.AddBitmap("Bitmaps/Soldier2_Down.bmp", RGB(255, 255, 255));
    peopleRU.AddBitmap("Bitmaps/Soldier2_RU.bmp", RGB(255, 255, 255));
    peopleLU.AddBitmap("Bitmaps/Soldier2_LU.bmp", RGB(255, 255, 255));
    peopleRD.AddBitmap("Bitmaps/Soldier2_RD.bmp", RGB(255, 255, 255));
    peopleLD.AddBitmap("Bitmaps/Soldier2_LD.bmp", RGB(255, 255, 255));
	peopleRUD.AddBitmap("Bitmaps/Soldier2_RUD.bmp", RGB(255, 255, 255));
	peopleLUD.AddBitmap("Bitmaps/Soldier2_LUD.bmp", RGB(255, 255, 255));
	peopleRDD.AddBitmap("Bitmaps/Soldier2_RDD.bmp", RGB(255, 255, 255));
	peopleLDD.AddBitmap("Bitmaps/Soldier2_LDD.bmp", RGB(255, 255, 255));
	peopleRUU.AddBitmap("Bitmaps/Soldier2_RUU.bmp", RGB(255, 255, 255));
	peopleLUU.AddBitmap("Bitmaps/Soldier2_LUU.bmp", RGB(255, 255, 255));
	peopleRDU.AddBitmap("Bitmaps/Soldier2_RDU.bmp", RGB(255, 255, 255));
	peopleLDU.AddBitmap("Bitmaps/Soldier2_LDU.bmp", RGB(255, 255, 255));
	//-------//
	//-----角色開火動畫---//
	peopleL_fire.AddBitmap("Bitmaps/Soldier2_Left_fire.bmp", RGB(255, 255, 255));
	peopleU_fire.AddBitmap("Bitmaps/Soldier2_Up_fire.bmp", RGB(255, 255, 255));
	peopleR_fire.AddBitmap("Bitmaps/Soldier2_Right_fire.bmp", RGB(255, 255, 255));
	peopleD_fire.AddBitmap("Bitmaps/Soldier2_Down_fire.bmp", RGB(255, 255, 255));
	peopleRU_fire.AddBitmap("Bitmaps/Soldier2_RU_fire.bmp", RGB(255, 255, 255));
	peopleLU_fire.AddBitmap("Bitmaps/Soldier2_LU_fire.bmp", RGB(255, 255, 255));
	peopleRD_fire.AddBitmap("Bitmaps/Soldier2_RD_fire.bmp", RGB(255, 255, 255));
	peopleLD_fire.AddBitmap("Bitmaps/Soldier2_LD_fire.bmp", RGB(255, 255, 255));
	peopleRUD_fire.AddBitmap("Bitmaps/Soldier2_RUD_fire.bmp", RGB(255, 255, 255));
	peopleLUD_fire.AddBitmap("Bitmaps/Soldier2_LUD_fire.bmp", RGB(255, 255, 255));
	peopleRDD_fire.AddBitmap("Bitmaps/Soldier2_RDD_fire.bmp", RGB(255, 255, 255));
	peopleLDD_fire.AddBitmap("Bitmaps/Soldier2_LDD_fire.bmp", RGB(255, 255, 255));
	peopleRUU_fire.AddBitmap("Bitmaps/Soldier2_RUU_fire.bmp", RGB(255, 255, 255));
	peopleLUU_fire.AddBitmap("Bitmaps/Soldier2_LUU_fire.bmp", RGB(255, 255, 255));
	peopleRDU_fire.AddBitmap("Bitmaps/Soldier2_RDU_fire.bmp", RGB(255, 255, 255));
	peopleLDU_fire.AddBitmap("Bitmaps/Soldier2_LDU_fire.bmp", RGB(255, 255, 255));
	//---//
	//-----路線圖片載入---///
    lineUP.AddBitmap(IDB_LINEUP, RGB(255, 255, 255));
    lineRight.AddBitmap(IDB_LINERIGHT, RGB(255, 255, 255));
    lineDown.AddBitmap(IDB_LINEDOWN, RGB(255, 255, 255));
    lineLeft.AddBitmap(IDB_LINELEFT, RGB(255, 255, 255));
    lineRD.AddBitmap(IDB_LINERD, RGB(255, 255, 255));
    lineRU.AddBitmap(IDB_LINERU, RGB(255, 255, 255));
    lineLD.AddBitmap(IDB_LINELD, RGB(255, 255, 255));
    lineLU.AddBitmap(IDB_LINELU, RGB(255, 255, 255));
    breakPoint.AddBitmap(IDB_BALL, RGB(255, 255, 255));
	//-----//
	//----武器圖片載入----//
	HK416_Photo.LoadBitmap("Bitmaps/HK416N.bmp");
	P9_Photo.LoadBitmap("Bitmaps/P9.bmp");
	//--------//
}

void Soldier::OnMove()
{
    const int STEP_SIZE = 4;
    isMoveNextIndex = false;
    if (isMovingLeft)
    {
        if (x > (index_x - 1) * SIZE)
            x -= STEP_SIZE;
        else
        {
            index_x--;
            isMoveNextIndex = true;
            way++;
        }
    }
    if (isMovingRight)
    {
        if (x < (index_x + 1) * SIZE)
            x += STEP_SIZE;
        else
        {
            index_x++;
            isMoveNextIndex = true;
            way++;
        }
    }
    if (isMovingUp)
    {
        if (y > (index_y - 1) * SIZE)
            y -= STEP_SIZE;
        else
        {
            index_y--;
            isMoveNextIndex = true;
            way++;
        }
    }
    if (isMovingDown)
    {
        if (y < (index_y + 1) * SIZE)
            y += STEP_SIZE;
        else
        {
            index_y++;
            isMoveNextIndex = true;
            way++;
        }
    }
    if (isMovingRightUp)
    {
        if (y > (index_y - 1) * SIZE && x < (index_x + 1) * SIZE)
        {
            y -= STEP_SIZE;
            x += STEP_SIZE;
        }
        else
        {
            index_y--;
            index_x++;
            isMoveNextIndex = true;
            way++;
        }
    }
    if (isMovingRightDown)
    {
        if (y < (index_y + 1) * SIZE && x < (index_x + 1) * SIZE)
        {
            y += STEP_SIZE;
            x += STEP_SIZE;
        }
        else
        {
            index_y++;
            index_x++;
            isMoveNextIndex = true;
            way++;
        }
    }
    if (isMovingLeftUp)
    {
        if (y > (index_y - 1) * SIZE && x > (index_x - 1) * SIZE)
        {
            y -= STEP_SIZE;
            x -= STEP_SIZE;
        }
        else
        {
            index_y--;
            index_x--;
            isMoveNextIndex = true;
            way++;
        }
    }
    if (isMovingLeftDown)
    {
        if (y < (index_y + 1) * SIZE && x > (index_x - 1) * SIZE)
        {
            y += STEP_SIZE;
            x -= STEP_SIZE;
        }
        else
        {
            index_y++;
            index_x--;
            isMoveNextIndex = true;
            way++;
        }
    }
}
void Soldier::OnShow()
{
    if (!roadLine.empty() && roadLine.back()->getWatch()!= -1)
    {
        int line_x = index_x, line_y = index_y;
        for (vector<Line*>::iterator iter = roadLine.begin(); iter != roadLine.end(); iter++)
        {
            DrawLineSecond(iter, &line_x, &line_y);
            DrawLineFirst(iter, &line_x, &line_y);
        }
        breakPoint.SetTopLeft(line_x * 40, line_y * 40);
        breakPoint.OnShow();
    }
    if (isWatchDown)
    {
        peopleD.SetTopLeft(x, y);
        peopleD.OnShow();
        direction = 4;
    }
    else if (isWatchLeft)
    {
        peopleL.SetTopLeft(x, y);
        peopleL.OnShow();
        direction = 6;
    }
    else if (isWatchRight)
    {
        peopleR.SetTopLeft(x, y);
        peopleR.OnShow();
        direction = 2;
    }
    else if (isWatchUp)
    {
        peopleU.SetTopLeft(x, y);
        peopleU.OnShow();
        direction = 0;
    }
    else if (way != roadLine.end())
    {
		if (lock_enemy != -1) direction = lock_enemy;
		else if ((*way)->getWatch()!= -1)direction = (*way)->getWatch();
        switch (direction)
        {
            case 0:
				if (isShoot) {
					peopleU_fire.SetTopLeft(x, y);
					peopleU_fire.OnShow();
				}
				else {
					peopleU.SetTopLeft(x, y);
					peopleU.OnShow();
				}
                break;
            case 1:
				if (isShoot) {
					peopleRU_fire.SetTopLeft(x, y);
					peopleRU_fire.OnShow();
				}
				else {
					peopleRU.SetTopLeft(x, y);
					peopleRU.OnShow();
				}
                break;
            case 2:
				if (isShoot) {
					peopleR_fire.SetTopLeft(x, y);
					peopleR_fire.OnShow();
				}
				else {
					peopleR.SetTopLeft(x, y);
					peopleR.OnShow();
				}
                break;
            case 3:
				if (isShoot) {
					peopleRD_fire.SetTopLeft(x, y);
					peopleRD_fire.OnShow();
				}
				else {
					peopleRD.SetTopLeft(x, y);
					peopleRD.OnShow();
				}
                break;
            case 4:
				if (isShoot) {
					peopleD_fire.SetTopLeft(x, y);
					peopleD_fire.OnShow();
				}
				else {
					peopleD.SetTopLeft(x, y);
					peopleD.OnShow();
				}
                break;
            case 5:
				if (isShoot) {
					peopleLD_fire.SetTopLeft(x, y);
					peopleLD_fire.OnShow();
				}
				else {
					peopleLD.SetTopLeft(x, y);
					peopleLD.OnShow();
				}
                break;
            case 6:
				if (isShoot) {
					peopleL_fire.SetTopLeft(x, y);
					peopleL_fire.OnShow();
				}
				else {
					peopleL.SetTopLeft(x, y);
					peopleL.OnShow();
				}
                break;
            case 7:
				if (isShoot) {
					peopleLU_fire.SetTopLeft(x, y);
					peopleLU_fire.OnShow();
				}
				else {
					peopleLU.SetTopLeft(x, y);
					peopleLU.OnShow();
				}
                break;
			case 8:
				if (isShoot) {
					peopleRUD_fire.SetTopLeft(x, y);
					peopleRUD_fire.OnShow();
				}
				else {
					peopleRUD.SetTopLeft(x, y);
					peopleRUD.OnShow();
				}
				break;
			case 9:
				if (isShoot) {
					peopleRUU_fire.SetTopLeft(x, y);
					peopleRUU_fire.OnShow();
				}
				else {
					peopleRUU.SetTopLeft(x, y);
					peopleRUU.OnShow();
				}
				break;
			case 10:
				if (isShoot) {
					peopleLUU_fire.SetTopLeft(x, y);
					peopleLUU_fire.OnShow();
				}
				else {
					peopleLUU.SetTopLeft(x, y);
					peopleLUU.OnShow();
				}
				break;
			case 11:
				if (isShoot) {
					peopleLUD_fire.SetTopLeft(x, y);
					peopleLUD_fire.OnShow();
				}
				else {
					peopleLUD.SetTopLeft(x, y);
					peopleLUD.OnShow();
				}

				break;
			case 12:
				if (isShoot) {
					peopleLDU_fire.SetTopLeft(x, y);
					peopleLDU_fire.OnShow();
				}
				else {
					peopleLDU.SetTopLeft(x, y);
					peopleLDU.OnShow();
				}
				break;
			case 13:
				if (isShoot) {
					peopleLDD_fire.SetTopLeft(x, y);
					peopleLDD_fire.OnShow();
				}
				else {
					peopleLDD.SetTopLeft(x, y);
					peopleLDD.OnShow();
				}
				break;
			case 14:
				if (isShoot) {
					peopleRDD_fire.SetTopLeft(x, y);
					peopleRDD_fire.OnShow();
				}
				else {
					peopleRDD.SetTopLeft(x, y);
					peopleRDD.OnShow();
				}
				break;
			case 15:
				if (isShoot) {
					peopleRDU_fire.SetTopLeft(x, y);
					peopleRDU_fire.OnShow();
				}
				else {
					peopleRDU.SetTopLeft(x, y);
					peopleRDU.OnShow();
				}
				break;

            default:
                break;
        }
    }
    else
    {
        switch (direction)
        {
		case 0:
			if (isShoot) {
				peopleU_fire.SetTopLeft(x, y);
				peopleU_fire.OnShow();
			}
			else {
				peopleU.SetTopLeft(x, y);
				peopleU.OnShow();
			}
			break;
		case 1:
			if (isShoot) {
				peopleRU_fire.SetTopLeft(x, y);
				peopleRU_fire.OnShow();
			}
			else {
				peopleRU.SetTopLeft(x, y);
				peopleRU.OnShow();
			}
			break;
		case 2:
			if (isShoot) {
				peopleR_fire.SetTopLeft(x, y);
				peopleR_fire.OnShow();
			}
			else {
				peopleR.SetTopLeft(x, y);
				peopleR.OnShow();
			}
			break;
		case 3:
			if (isShoot) {
				peopleRD_fire.SetTopLeft(x, y);
				peopleRD_fire.OnShow();
			}
			else {
				peopleRD.SetTopLeft(x, y);
				peopleRD.OnShow();
			}
			break;

		case 4:
			if (isShoot) {
				peopleD_fire.SetTopLeft(x, y);
				peopleD_fire.OnShow();
			}
			else {
				peopleD.SetTopLeft(x, y);
				peopleD.OnShow();
			}
			break;

		case 5:
			if (isShoot) {
				peopleLD_fire.SetTopLeft(x, y);
				peopleLD_fire.OnShow();
			}
			else {
				peopleLD.SetTopLeft(x, y);
				peopleLD.OnShow();
			}
			break;

		case 6:
			if (isShoot) {
				peopleL_fire.SetTopLeft(x, y);
				peopleL_fire.OnShow();
			}
			else {
				peopleL.SetTopLeft(x, y);
				peopleL.OnShow();
			}
			break;

		case 7:
			if (isShoot) {
				peopleLU_fire.SetTopLeft(x, y);
				peopleLU_fire.OnShow();
			}
			else {
				peopleLU.SetTopLeft(x, y);
				peopleLU.OnShow();
			}
			break;
		case 8:
			if (isShoot) {
				peopleRUD_fire.SetTopLeft(x, y);
				peopleRUD_fire.OnShow();
			}
			else {
				peopleRUD.SetTopLeft(x, y);
				peopleRUD.OnShow();
			}
			break;
		case 9:
			if (isShoot) {
				peopleRUU_fire.SetTopLeft(x, y);
				peopleRUU_fire.OnShow();
			}
			else {
				peopleRUU.SetTopLeft(x, y);
				peopleRUU.OnShow();
			}
			break;
		case 10:
			if (isShoot) {
				peopleLUU_fire.SetTopLeft(x, y);
				peopleLUU_fire.OnShow();
			}
			else {
				peopleLUU.SetTopLeft(x, y);
				peopleLUU.OnShow();
			}
			break;
		case 11:
			if (isShoot) {
				peopleLUD_fire.SetTopLeft(x, y);
				peopleLUD_fire.OnShow();
			}
			else {
				peopleLUD.SetTopLeft(x, y);
				peopleLUD.OnShow();
			}
			break;
		case 12:
			if (isShoot) {
				peopleLDU_fire.SetTopLeft(x, y);
				peopleLDU_fire.OnShow();
			}
			else {
				peopleLDU.SetTopLeft(x, y);
				peopleLDU.OnShow();
			}
			break;
		case 13:
			if (isShoot) {
				peopleLDD_fire.SetTopLeft(x, y);
				peopleLDD_fire.OnShow();
			}
			else {
				peopleLDD.SetTopLeft(x, y);
				peopleLDD.OnShow();
			}
			break;
		case 14:
			if (isShoot) {
				peopleRDD_fire.SetTopLeft(x, y);
				peopleRDD_fire.OnShow();
			}
			else {
				peopleRDD.SetTopLeft(x, y);
				peopleRDD.OnShow();
			}
			break;
		case 15:
			if (isShoot) {
				peopleRDU_fire.SetTopLeft(x, y);
				peopleRDU_fire.OnShow();
			}
			else {
				peopleRDU.SetTopLeft(x, y);
				peopleRDU.OnShow();
			}
				break;
            default:
                break;
        }
    }
	switch (gun)
	{
	case 0:
		HK416_Photo.SetTopLeft(0, 880);
		HK416_Photo.ShowBitmap();
		break;
	case 1:
		P9_Photo.SetTopLeft(0, 880);
		P9_Photo.ShowBitmap();
		break;
	default:
		break;
	}
	isShoot = false;
}
#pragma endregion

#pragma region Get
int Soldier::GetX1()
{
    return x;
}

int Soldier::GetY1()
{
    return y;
}

int Soldier::GetX2()
{
    return x + peopleU.Width();
}

int Soldier::GetY2()
{
    return y + peopleU.Height();
}
int Soldier::GetIndexX()
{
    return index_x;
}
int Soldier::GetIndexY()
{
    return index_y;
}
#pragma endregion

#pragma region Move
void Soldier::MoveL(bool flag, CGameMap* map)
{
    if (flag && isMovingLeft)
    {
        if (!roadLine.empty())
        {
			delete *roadLine.begin();
            roadLine.erase(roadLine.begin());
            way = roadLine.begin();
        }
        map->SetIndexValue(index_x + 1, index_y, 0);
        isMoveNextIndex = false;
    }
}
void Soldier::MoveR(bool flag, CGameMap* map)
{
    if (flag && isMovingRight)
    {
        if (!roadLine.empty())
        {
			delete *roadLine.begin();
            roadLine.erase(roadLine.begin());
            way = roadLine.begin();
        }
        map->SetIndexValue(index_x - 1, index_y, 0);
        isMoveNextIndex = false;
    }
}
void Soldier::MoveU(bool flag, CGameMap* map)
{
    if (flag && isMovingUp)
    {
        if (!roadLine.empty())
        {
			delete *roadLine.begin();
            roadLine.erase(roadLine.begin());
            way = roadLine.begin();
        }

        map->SetIndexValue(index_x, index_y + 1, 0);
        isMoveNextIndex = false;
    }
}
void Soldier::MoveD(bool flag, CGameMap* map)
{
    if (flag && isMovingDown)
    {
        if (!roadLine.empty())
        {
			delete *roadLine.begin();
            roadLine.erase(roadLine.begin());
            way = roadLine.begin();
        }
        map->SetIndexValue(index_x, index_y - 1, 0);
        isMoveNextIndex = false;
    }
}
void Soldier::MoveLU(bool flag, CGameMap* map)
{
    if (flag && isMovingLeftUp)
    {
        if (!roadLine.empty())
        {
			delete *roadLine.begin();
            roadLine.erase(roadLine.begin());
            way = roadLine.begin();
        }
        map->SetIndexValue(index_x + 1, index_y + 1, 0);
        isMoveNextIndex = false;
    }
}
void Soldier::MoveLD(bool flag, CGameMap* map)
{
    if (flag && isMovingLeftDown)
    {
        if (!roadLine.empty())
        {
			delete *roadLine.begin();
            roadLine.erase(roadLine.begin());
            way = roadLine.begin();
        }
        map->SetIndexValue(index_x + 1, index_y - 1, 0);
        isMoveNextIndex = false;
    }
}
void Soldier::MoveRU(bool flag, CGameMap* map)
{
    if (flag && isMovingRightUp)
    {
        if (!roadLine.empty())
        {
			delete *roadLine.begin();
            roadLine.erase(roadLine.begin());
            way = roadLine.begin();
        }

        map->SetIndexValue(index_x - 1, index_y + 1, 0);
        isMoveNextIndex = false;
    }
}
void Soldier::MoveRD(bool flag, CGameMap* map)
{
    if (flag && isMovingRightDown)
    {
        if (!roadLine.empty())
        {
			delete *roadLine.begin();
            roadLine.erase(roadLine.begin());
            way = roadLine.begin();
        }
        map->SetIndexValue(index_x - 1, index_y - 1, 0);
        isMoveNextIndex = false;
    }
}
void Soldier::MoveLeftIndex()
{
    index_x -= 1;
}
void Soldier::MoveRightIndex()
{
    index_x += 1;
}
void Soldier::MoveUpIndex()
{
    index_y -= 1;
}
void Soldier::MoveDownIndex()
{
    index_y += 1;
}
#pragma endregion

#pragma region Set
void Soldier::SetMovingRightDown(bool flag)
{
    isMovingRightDown = flag;
}
void Soldier::SetMovingLeftDown(bool flag)
{
    isMovingLeftDown = flag;
}
void Soldier::SetMovingRightUp(bool flag)
{
    isMovingRightUp = flag;
}
void Soldier::SetMovingLeftUp(bool flag)
{
    isMovingLeftUp = flag;
}
void Soldier::SetMovingDown(bool flag)
{
    isMovingDown = flag;
}
void Soldier::SetMovingLeft(bool flag)
{
    isMovingLeft = flag;
}
void Soldier::SetMovingRight(bool flag)
{
    isMovingRight = flag;
}
void Soldier::SetMovingUp(bool flag)
{
    isMovingUp = flag;
}
void Soldier::SetChoosen(bool flag)
{
    isChoosen = flag;
}
void Soldier::SetRoadLine(bool flag)
{
    isSetRoadLine = flag;
}
void Soldier::SetInRoadLine(bool flag)
{
	isInRoadLine = flag;
}
void Soldier::SetAction(bool flag)
{
    isSetAction = flag;
}
void Soldier::SetWatchUp(bool flag)
{
    isWatchUp = flag;
}
void Soldier::SetWatchDown(bool flag)
{
    isWatchDown = flag;
}
void Soldier::SetWatchRight(bool flag)
{
    isWatchRight = flag;
}
void Soldier::SetWatchLeft(bool flag)
{
    isWatchLeft = flag;
}
void Soldier::SetWatchLeftUp(bool flag)
{
    isWatchLeftUp = flag;
}
void Soldier::SetWatchRightUp(bool flag)
{
    isWatchRightUp = flag;
}
void Soldier::SetWatchRightDown(bool flag)
{
    isWatchRightDown = flag;
}
void Soldier::SetWatchLeftDown(bool flag)
{
    isWatchLeftDown = flag;
}
void Soldier::SetNextDoor(bool flag)
{
    isNextDoor = flag;
}
void Soldier::SetXY(int nx, int ny)
{
    x = nx;
    y = ny;
}
void Soldier::GetHurt(int damage)
{
	lifepoint -= damage;
}
int Soldier::GetLifePoint()
{
	return lifepoint;
}
#pragma endregion

#pragma region shoot
void Soldier::searchEnemy(CGameMap* map, vector<Enemy*>& enemys, vector<Furniture*>& furniture)
{
    double dx, dy;
    double Lx, Ly;
    double rotate_start, rotate_end;
	int Light_ix, Light_iy, Lix, Liy;
    double pi;
	double enemy_dir=-1;
	Enemy *_target = NULL;
	//--依方向設定起始角度--//
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
		case 8:
			rotate_start = 337.5;
			break;
		case 9:
			rotate_start = 22.5;
			break;
		case 10:
			rotate_start = 67.5;
			break;
		case 11:
			rotate_start = 112.5;
			break;
		case 12:
			rotate_start = 157.5;
			break;
		case 13:
			rotate_start = 202.5;
			break;
		case 14:
			rotate_start = 247.5;
			break;
		case 15:
			rotate_start = 292.5;
			break;
    }
	pi = 3.14159265 / 180.0;	//定義角動量
    rotate_end = rotate_start + 90;	//設定終止角度
    for (double rotate = rotate_start; rotate <= rotate_end; rotate += 0.125)	//每0.125度掃描
    {
        Lx = x+20 ;
        Ly = y+20 ;
		Lix = static_cast<int>(Lx) / 40;
        Liy = static_cast<int>(Ly) / 40;
        dx = 20 * cos(rotate * pi);
        dy = -20 * sin(rotate * pi);
		Light_ix = static_cast<int>(Lx)/20;
		Light_iy = static_cast<int>(Ly)/20;
        do    //以目前角度做直線值至碰到障礙物
        {
            Lix = static_cast<int>( Lx )/ 40;
            Liy = static_cast<int>( Ly )/ 40;
			Light_ix = static_cast<int>(Lx) / 20;
			Light_iy = static_cast<int>( Ly) / 20;
            if (map->GetIndexValue(Lix, Liy) == 2)	//如果看見敵人
            {
                for (vector<Enemy*>::iterator iter = enemys.begin(); iter != enemys.end(); iter++)
                {
                    if ((*iter)->GetIndexX() == Lix && (*iter)->GetIndexY() == Liy)
                    {
                        (*iter)->SetIsSaw(true);
						if(_target==NULL)	//設為目標
							_target = (*iter);
						if (enemy_dir == -1)	//記住目前角度
							enemy_dir = rotate;
                    }
                }
            }
			else if (map->GetIndexValue(Lix, Liy) == 3) {	//家具發亮
				for (vector<Furniture*>::iterator iter = furniture.begin(); iter != furniture.end(); iter++)
				{
					if ((*iter)->GetX() == Lix && (*iter)->GetY() == Liy)
					{
						(*iter)->SetIsSaw(true);
						break;
					}
				}
			}
			map->SetBackLight(Light_iy, Light_ix, true);	//地板發亮
            Lx += dx;
            Ly += dy;
        } while (map->GetIndexValue(Lix, Liy) < 5&&(Lx>0&&Lx<ROW*SIZE&&Ly>0&&Ly< COL*SIZE));//地圖範圍內掃描
	}
		target = _target;
		if (enemy_dir != -1)	//將正面朝敵
		{
			if (enemy_dir >= 360)enemy_dir -= 360;
			if (enemy_dir <= 11.25 || enemy_dir > 348.75)lock_enemy = 2;
			else if (enemy_dir <= 33.75 && enemy_dir > 11.25)lock_enemy = 8;
			else if (enemy_dir <= 56.25 && enemy_dir > 33.75)lock_enemy = 1;
			else if (enemy_dir <= 78.75 && enemy_dir > 56.25)lock_enemy = 9;
			else if (enemy_dir <= 101.25 && enemy_dir > 78.75)lock_enemy = 0;
			else if (enemy_dir <= 123.75 && enemy_dir > 101.25)lock_enemy = 10;
			else if (enemy_dir <= 146.25 && enemy_dir > 123.75)lock_enemy = 7;
			else if (enemy_dir <= 168.75 && enemy_dir > 146.25)lock_enemy = 11;
			else if (enemy_dir <= 191.25 && enemy_dir > 168.75)lock_enemy = 6;
			else if (enemy_dir <= 213.75 && enemy_dir > 191.25)lock_enemy = 12;
			else if (enemy_dir <= 236.25 && enemy_dir > 213.75)lock_enemy = 5;
			else if (enemy_dir <= 258.75 && enemy_dir > 236.25)lock_enemy = 13;
			else if (enemy_dir <= 281.25 && enemy_dir > 258.75)lock_enemy = 4;
			else if (enemy_dir <= 303.75 && enemy_dir > 281.25)lock_enemy = 14;
			else if (enemy_dir <= 326.25 && enemy_dir > 303.75)lock_enemy = 3;
			else if (enemy_dir <= 348.75 && enemy_dir > 326.25)lock_enemy = 15;
		}
		else 
		{
			lock_enemy = -1; 
		}
}

void Soldier::attackEnemy()
{
	if (target != NULL)
	{
		if (shoot_count_time > reaction_time)	//瞄準時間
		{
			this->shoot();
			shoot_count_time = 5;	//第二次對已瞄準敵人開火延遲
		}
		shoot_count_time++;
	}
	else
	{
		shoot_count_time = 0;
	}
}
void Soldier::shoot()
{
	int damage=weapon->GetDamage();
	isShoot = true;
	if (target->GetLifePoint() <= 0) {	//敵人陣亡不能繼續鞭屍
		target = NULL;
	}
	else
	{
		unique_ptr<thread> fire(new thread(&Weapon::Fire, weapon));
		target->GetHurt(damage);
		if (fire->joinable())fire->join();
	}
	
}
#pragma endregion

#pragma region Action
void Soldier::SetRoadLine(int mouse_x, int mouse_y, CGameMap* map)
{
    if (roadLine.empty())	//起始路線座標設定
    {
        moving_index_x = index_x;
        moving_index_y = index_y;
    }
    while (mouse_y > 0 && mouse_x < ROW && map->GetIndexValue(moving_index_x + 1, moving_index_y - 1) < 3 && mouse_x > moving_index_x && mouse_y < moving_index_y)
    {
        if (!roadLine.empty() && roadLine.back()->getWatch() == 5)
        {
			delete roadLine.back();
            roadLine.pop_back();
			if (roadLine.size() == 0)
			{
				TRACE("empty\n");
				roadLine.push_back(new Line(-1));
			}
        }
		else {
			if (!roadLine.empty() && roadLine.back()->getWatch() == -1)
			{
				delete roadLine.back();
				roadLine.pop_back();
			}
				
			roadLine.push_back(new Line(1));
		}

        moving_index_x += 1;
        moving_index_y -= 1;
    }
    while (mouse_y < COL && mouse_x < ROW && map->GetIndexValue(moving_index_x + 1, moving_index_y + 1) < 2 && mouse_x > moving_index_x && mouse_y >  moving_index_y)
    {
        if (!roadLine.empty() && roadLine.back()->getWatch() == 7)
        {
			delete roadLine.back();
            roadLine.pop_back();
			if (roadLine.size() == 0)
			{
				TRACE("empty\n");
				roadLine.push_back(new Line(-1));
			}
		}
		else {
			if (!roadLine.empty() && roadLine.back()->getWatch() == -1)
			{
				delete roadLine.back();
				roadLine.pop_back();
			}
			roadLine.push_back(new Line(3));
		}
        moving_index_x += 1;
        moving_index_y += 1;
    }
    while (mouse_y < COL && mouse_x > 0 && map->GetIndexValue(moving_index_x - 1, moving_index_y + 1) < 3 && mouse_x <  moving_index_x && mouse_y >  moving_index_y)
    {
        if (!roadLine.empty() && roadLine.back()->getWatch() == 1)
        {
			delete roadLine.back();
            roadLine.pop_back();
			if (roadLine.size() == 0)
			{
				TRACE("empty\n");
				roadLine.push_back(new Line(-1));
			}
		}
		else {
			if (!roadLine.empty() && roadLine.back()->getWatch() == -1)
			{
				delete roadLine.back();
				roadLine.pop_back();
			}
			roadLine.push_back(new Line(5));
		}
        moving_index_x -= 1;
        moving_index_y += 1;
    }
    while (mouse_y > 0 && mouse_x > 0 && map->GetIndexValue(moving_index_x - 1, moving_index_y - 1) < 3 && mouse_x < moving_index_x && mouse_y < moving_index_y)
    {
        if (!roadLine.empty() && roadLine.back()->getWatch() == 3)
        {
			delete roadLine.back();
            roadLine.pop_back();
			if (roadLine.size() == 0)
			{
				TRACE("empty\n");
				roadLine.push_back(new Line(-1));
			}
		}
		else {
			if (!roadLine.empty() && roadLine.back()->getWatch() == -1)
			{
				delete roadLine.back();
				roadLine.pop_back();
			}
			roadLine.push_back(new Line(7));
		}
        moving_index_x -= 1;
        moving_index_y -= 1;
    }
    while (mouse_x < ROW && map->GetIndexValue(moving_index_x + 1, moving_index_y) < 3 && mouse_x > moving_index_x)
    {
        if (!roadLine.empty() && roadLine.back()->getWatch() == 6)
        {
			delete roadLine.back();
            roadLine.pop_back();
			if (roadLine.size() == 0)
			{
				TRACE("empty\n");
				roadLine.push_back(new Line(-1));
			}
		}
		else {
			if (!roadLine.empty() && roadLine.back()->getWatch() == -1)
			{
				delete roadLine.back();
				roadLine.pop_back();
			}
			roadLine.push_back(new Line(2));
		}
        moving_index_x += 1;
    }
    while (mouse_x > 0 && map->GetIndexValue(moving_index_x - 1, moving_index_y) < 3 && mouse_x < moving_index_x)
    {
        if (!roadLine.empty() && roadLine.back()->getWatch() == 2)
        {
			delete roadLine.back();
            roadLine.pop_back();
			if (roadLine.size() == 0)
			{
				TRACE("empty\n");
				roadLine.push_back(new Line(-1));
			}
		}
		else {
			if (!roadLine.empty() && roadLine.back()->getWatch() == -1)
			{
				delete roadLine.back();
				roadLine.pop_back();
			}
			roadLine.push_back(new Line(6));
		}
        moving_index_x -= 1;
    }
    while (mouse_y < COL && map->GetIndexValue(moving_index_x, moving_index_y + 1) < 3 && mouse_y > moving_index_y)
    {
        if (!roadLine.empty() && roadLine.back()->getWatch() == 0)
        {
			delete roadLine.back();
            roadLine.pop_back();
			if (roadLine.size() == 0)
			{
				TRACE("empty\n");
				roadLine.push_back(new Line(-1));
			}
		}
		else {
			if (!roadLine.empty() && roadLine.back()->getWatch() == -1)
			{
				delete roadLine.back();
				roadLine.pop_back();
			}
			roadLine.push_back(new Line(4));
		}
        moving_index_y += 1;
    }
    while (mouse_y > 0 && map->GetIndexValue(moving_index_x, moving_index_y - 1) < 3 && mouse_y < moving_index_y)
    {
        if (!roadLine.empty() && roadLine.back()->getWatch() == 4)
        {
			delete roadLine.back();
            roadLine.pop_back();
			if (roadLine.size() == 0)
			{
				TRACE("empty\n");
				roadLine.push_back(new Line(-1));
			}
		}
		else {
			if (!roadLine.empty() && roadLine.back()->getWatch() == -1)
			{
				delete roadLine.back();
				roadLine.pop_back();
			}
			roadLine.push_back(new Line(0));
		}
        moving_index_y -= 1;
    }
    if (!roadLine.empty())
        way = roadLine.begin();
}
void Soldier::ChangeGun(int num)
{
	gun = num;
	weapon = equipment[gun];
}
const vector<Line*>& Soldier::GetRoadLine()
{
    return roadLine;
}
const int  Soldier::GetWay()
{
    if (way != roadLine.end())
    {
        return (*way)->getWatch();
    }
    return -1;
}
#pragma endregion

#pragma region Bool
bool Soldier::GetIsMoveNext()
{
    return isMoveNextIndex;
}
bool Soldier::IsChoosen()
{
    return isChoosen;
}
bool Soldier::IsSetRoadLine(CPoint& point)
{
    int Mx = point.x / 40, My = point.y / 40;
    if (roadLine.empty())
        return (Mx == index_x && My == index_y);
    else
    {
        int Rx = index_x, Ry = index_y;
        for (vector<Line*>::iterator iter = roadLine.begin(); iter != roadLine.end(); iter++)
        {
            switch ((*iter)->getWatch())
            {
                case 0:
                    Ry--;
                    break;

                case 1:
                    Ry--;
                    Rx++;
                    break;

                case 2:
                    Rx++;
                    break;

                case 3:
                    Rx++;
                    Ry++;
                    break;

                case 4:
                    Ry++;
                    break;

                case 5:
                    Rx--;
                    Ry++;
                    break;

                case 6:
                    Rx--;
                    break;

                case 7:
                    Rx--;
                    Ry--;
                    break;

                default:
                    break;
            }
        }
        return Mx == Rx && My == Ry;
    }
}
bool Soldier::IsSetAction(CPoint& point)
{
    int Mx = point.x / 40, My = point.y / 40;
    int Rx = index_x, Ry = index_y;
    for (vector<Line*>::iterator iter = roadLine.begin(); iter != roadLine.end(); iter++)
    {
        switch ((*iter)->getWatch())
        {
            case 0:
                Ry--;
                break;
            case 1:
                Ry--;
                Rx++;
                break;
            case 2:
                Rx++;
                break;
            case 3:
                Rx++;
                Ry++;
                break;
            case 4:
                Ry++;
                break;
            case 5:
                Rx--;
                Ry++;
                break;
            case 6:
                Rx--;
                break;
            case 7:
                Rx--;
                Ry--;
                break;
            default:
                break;
        }
        if (Mx == Rx && My == Ry) return true;
    }
    return false;
}
bool Soldier::IsInRoadLine()
{
	return isInRoadLine;
}
#pragma endregion

#pragma region Draw
void Soldier::DrawLineFirst(vector<Line*>::iterator iter, int* line_x, int* line_y)
{
    switch ((*iter)->getWatch())
    {
        case 0:
            *line_y -= 1;
            lineDown.SetTopLeft(*line_x * 40, *line_y * 40);
            lineDown.OnShow();
            break;
        case 1:
            *line_x += 1;
            *line_y -= 1;
            lineLD.SetTopLeft(*line_x * 40, *line_y * 40);
            lineLD.OnShow();
            break;
        case 2:
            *line_x += 1;
            lineLeft.SetTopLeft(*line_x * 40, *line_y * 40);
            lineLeft.OnShow();
            break;
        case 3:
            *line_x += 1;
            *line_y += 1;
            lineLU.SetTopLeft(*line_x * 40, *line_y * 40);
            lineLU.OnShow();
            break;
        case 4:
            *line_y += 1;
            lineUP.SetTopLeft(*line_x * 40, *line_y * 40);
            lineUP.OnShow();
            break;
        case 5:
            *line_x -= 1;
            *line_y += 1;
            lineRU.SetTopLeft(*line_x * 40, *line_y * 40);
            lineRU.OnShow();
            break;
        case 6:
            *line_x -= 1;
            lineRight.SetTopLeft(*line_x * 40, *line_y * 40);
            lineRight.OnShow();
            break;
        case 7:
            *line_x -= 1;
            *line_y -= 1;
            lineRD.SetTopLeft(*line_x * 40, *line_y * 40);
            lineRD.OnShow();
            break;
        default:
            break;
    }
}
void Soldier::DrawLineSecond(vector<Line*>::iterator iter, int* line_x, int* line_y)
{
    switch ((*iter)->getWatch())
    {
        case 0:
            lineUP.SetTopLeft(*line_x * 40, *line_y * 40);
            lineUP.OnShow();
            break;
        case 1:
            lineRU.SetTopLeft(*line_x * 40, *line_y * 40);
            lineRU.OnShow();
            break;
        case 2:
            lineRight.SetTopLeft(*line_x * 40, *line_y * 40);
            lineRight.OnShow();
            break;
        case 3:
            lineRD.SetTopLeft(*line_x * 40, *line_y * 40);
            lineRD.OnShow();
            break;
        case 4:
            lineDown.SetTopLeft(*line_x * 40, *line_y * 40);
            lineDown.OnShow();
            break;
        case 5:
            lineLD.SetTopLeft(*line_x * 40, *line_y * 40);
            lineLD.OnShow();
            break;
        case 6:
            lineLeft.SetTopLeft(*line_x * 40, *line_y * 40);
            lineLeft.OnShow();
            break;
        case 7:
            lineLU.SetTopLeft(*line_x * 40, *line_y * 40);
            lineLU.OnShow();
            break;
        default:
            break;
    }
}
#pragma endregion
void Soldier::superman()
{
	lifepoint = 100000;
}
}