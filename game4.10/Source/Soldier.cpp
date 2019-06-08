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
Soldier::Soldier()
{
    Initialize();
}
Soldier::~Soldier()
{
	TRACE("~Soldier()\n");
	TRACE("weapon size:%d\n",sizeof(Weapon));
	delete weapon;
	weapon = NULL;
	target = NULL;
}
void Soldier::Initialize()
{
    const int X_POS = SIZE;
    const int Y_POS = SIZE;
    x = X_POS;
    y = Y_POS;
	gun = 0;
    index_x = index_y = 1;
	x1 = x2 = y1 = y2 = x11 = x12 = x21 = x22 = y11 = y12 = y21 = y22= end_x= end_y = 0;
    isSetRoadLine = true;
    isChoosen = isWatchDown = isWatchLeft = isWatchLeftDown = isWatchLeftUp = isWatchRight = isWatchRightDown = isWatchRightUp = isWatchUp = isSetAction = false;
    isMovingLeft = isMovingRight = isMovingUp = isMovingDown = isMovingLeftDown = isMovingLeftUp = isMovingRightDown = isMovingRightUp = false;
    isMoveNextIndex = isInRoadLine= false;
    way = roadLine.end();
    direction = 2;

	equipment[0] = new HK416();
	equipment[1] = new P9();
	weapon = new HK416();
	target = NULL;
}

void Soldier::LoadBitmap()
{
    peopleL.AddBitmap("Bitmaps/Soldier2_Left.bmp", RGB(255, 255, 255));
    peopleU.AddBitmap("Bitmaps/Soldier2_Up.bmp", RGB(255, 255, 255));
    peopleR.AddBitmap("Bitmaps/Soldier2_Right.bmp", RGB(255, 255, 255));
    peopleD.AddBitmap("Bitmaps/Soldier2_Down.bmp", RGB(255, 255, 255));
    peopleRU.AddBitmap("Bitmaps/Soldier2_RU.bmp", RGB(255, 255, 255));
    peopleLU.AddBitmap("Bitmaps/Soldier2_LU.bmp", RGB(255, 255, 255));
    peopleRD.AddBitmap("Bitmaps/Soldier2_RD.bmp", RGB(255, 255, 255));
    peopleLD.AddBitmap("Bitmaps/Soldier2_LD.bmp", RGB(255, 255, 255));
    lineUP.AddBitmap(IDB_LINEUP, RGB(255, 255, 255));
    lineRight.AddBitmap(IDB_LINERIGHT, RGB(255, 255, 255));
    lineDown.AddBitmap(IDB_LINEDOWN, RGB(255, 255, 255));
    lineLeft.AddBitmap(IDB_LINELEFT, RGB(255, 255, 255));
    lineRD.AddBitmap(IDB_LINERD, RGB(255, 255, 255));
    lineRU.AddBitmap(IDB_LINERU, RGB(255, 255, 255));
    lineLD.AddBitmap(IDB_LINELD, RGB(255, 255, 255));
    lineLU.AddBitmap(IDB_LINELU, RGB(255, 255, 255));
    breakPoint.AddBitmap(IDB_BALL, RGB(255, 255, 255));
	HK416_Photo.LoadBitmap("Bitmaps/HK416N.bmp");
	P9_Photo.LoadBitmap("Bitmaps/P9.bmp");
}

void Soldier::OnMove()
{
    const int STEP_SIZE = 4;
    isMoveNextIndex = false;
    //TRACE("YYYYYYYYYYYYYYY   \n");

    //if(way==roadLine.end())
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

    if (way == roadLine.end())
    {
        roadLine.clear();
        roadLine.resize(0);
        way = roadLine.end();
    }

    //x:({x}), y:({y}),index_x:({index_x}),index_y({index_y})
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
		if ((*way)->getWatch()!= -1)direction = (*way)->getWatch();

        switch (direction)
        {
            case 0:
                peopleU.SetTopLeft(x, y);
                peopleU.OnShow();
                break;

            case 1:
                peopleRU.SetTopLeft(x, y);
                peopleRU.OnShow();
                break;

            case 2:
                peopleR.SetTopLeft(x, y);
                peopleR.OnShow();
                break;

            case 3:
                peopleRD.SetTopLeft(x, y);
                peopleRD.OnShow();
                break;

            case 4:
                peopleD.SetTopLeft(x, y);
                peopleD.OnShow();
                break;

            case 5:
                peopleLD.SetTopLeft(x, y);
                peopleLD.OnShow();
                break;

            case 6:
                peopleL.SetTopLeft(x, y);
                peopleL.OnShow();
                break;

            case 7:
                peopleLU.SetTopLeft(x, y);
                peopleLU.OnShow();
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
                peopleU.SetTopLeft(x, y);
                peopleU.OnShow();
                break;

            case 1:
                peopleRU.SetTopLeft(x, y);
                peopleRU.OnShow();
                break;

            case 2:
                peopleR.SetTopLeft(x, y);
                peopleR.OnShow();
                break;

            case 3:
                peopleRD.SetTopLeft(x, y);
                peopleRD.OnShow();
                break;

            case 4:
                peopleD.SetTopLeft(x, y);
                peopleD.OnShow();
                break;

            case 5:
                peopleLD.SetTopLeft(x, y);
                peopleLD.OnShow();
                break;

            case 6:
                peopleL.SetTopLeft(x, y);
                peopleL.OnShow();
                break;

            case 7:
                peopleLU.SetTopLeft(x, y);
                peopleLU.OnShow();
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

#pragma endregion

#pragma region shoot
void Soldier::searchEnemy(CGameMap* map, vector<Enemy*>& enemys)
{
    double dx, dy;
    double Lx, Ly;
    double rotate_start, rotate_end;
	int Light_ix, Light_iy, Lix, Liy;
    double pi;
	Enemy *_target = NULL;

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
	//TRACE("HEHEHEHEHEHE\n");
    for (double rotate = rotate_start; rotate <= rotate_end; rotate += 0.02)
    {
        Lx = x+20 ;
        Ly = y+20 ;
		Lix = static_cast<int>(Lx) / 40;
        Liy = static_cast<int>( Ly) / 40;
        
        dx = 20 * cos(rotate * pi);
        dy = -20 * sin(rotate * pi);
		//------µø³¥
		Light_ix = static_cast<int>(Lx) / 20;
		Light_iy = static_cast<int>(Ly )/ 20;
		
        //TRACE("Search:%d %d %d\n", dx, dy, rotate);

        do
        {
            //TRACE("Search:%d %d %d\n", Lix, Liy, rotate);
            Lix = static_cast<int>( Lx )/ 40;
            Liy = static_cast<int>( Ly )/ 40;
			Light_ix = static_cast<int>(Lx) / 20;
			Light_iy = static_cast<int>( Ly) / 20;
            if (map->GetIndexValue(Lix, Liy) == 2)
            {
                for (vector<Enemy*>::iterator iter = enemys.begin(); iter != enemys.end(); iter++)
                {
                    if ((*iter)->GetIndexX() == Lix && (*iter)->GetIndexY() == Liy)
                    {
                        //TRACE("GOT YOU\n");
                        (*iter)->SetIsSaw(true);
						if(_target==NULL)
							_target = (*iter);
                    }
                }
            }
			map->SetBackLight(Light_iy, Light_ix, true);
            Lx += dx;
            Ly += dy;
        } while (map->GetIndexValue(Lix, Liy) < 5);
	}
	if(_target!=NULL)
		target = _target;
}

void Soldier::attackEnemy()
{
	static int aim_time=0;
	if (target != NULL)
	{
		//TRACE("WTF:%d\n", aim_time);
		if (aim_time > 10)
		{
			//TRACE("SHOOT!!!\n");
			
			this->shoot();
			aim_time = 0;
		}
		aim_time++;
	}
	else
	{
		aim_time = 0;
	}
}
void Soldier::shoot()
{
	int damage=weapon->GetDamage();
	TRACE("AAAA\n");
	unique_ptr<thread> fire(new thread(&Weapon::Fire, weapon));
	target->Hurt(damage);
	if (target->GetLifePoint() <= 0) {
		target = NULL;
	}
	if (fire->joinable())fire->join();
	
}
#pragma endregion

#pragma region Action
void Soldier::SetRoadLine(int mouse_x, int mouse_y, CGameMap* map)
{

    if (roadLine.empty())
    {
        moving_index_x = index_x;
        moving_index_y = index_y;
    }

   // TRACE("Mouse %d %d\n", mouse_x, mouse_y);
   // TRACE("moving_index position in array: %d %d\n", moving_index_x, moving_index_y);

    while (mouse_y > 0 && mouse_x < ROW && map->GetIndexValue(moving_index_x + 1, moving_index_y - 1) < 3 && mouse_x > moving_index_x && mouse_y < moving_index_y)
    {
        if (!roadLine.empty() && roadLine.back()->getWatch() == 5)
        {
            roadLine.pop_back();
			if (roadLine.size() == 0)
			{
				TRACE("empty\n");
				roadLine.push_back(new Line(-1));
			}
        }
		else {
			if (!roadLine.empty() && roadLine.back()->getWatch() == -1) roadLine.pop_back();
			roadLine.push_back(new Line(1));
		}

        moving_index_x += 1;
        moving_index_y -= 1;
    }

    while (mouse_y < COL && mouse_x < ROW && map->GetIndexValue(moving_index_x + 1, moving_index_y + 1) < 3 && mouse_x > moving_index_x && mouse_y >  moving_index_y)
    {
        if (!roadLine.empty() && roadLine.back()->getWatch() == 7)
        {
            roadLine.pop_back();
			if (roadLine.size() == 0)
			{
				TRACE("empty\n");
				roadLine.push_back(new Line(-1));
			}
		}
		else {
			if (!roadLine.empty() && roadLine.back()->getWatch() == -1) roadLine.pop_back();
			roadLine.push_back(new Line(3));
		}
        moving_index_x += 1;
        moving_index_y += 1;
    }

    while (mouse_y < COL && mouse_x > 0 && map->GetIndexValue(moving_index_x - 1, moving_index_y + 1) < 3 && mouse_x <  moving_index_x && mouse_y >  moving_index_y)
    {
        if (!roadLine.empty() && roadLine.back()->getWatch() == 1)
        {
            roadLine.pop_back();
			if (roadLine.size() == 0)
			{
				TRACE("empty\n");
				roadLine.push_back(new Line(-1));
			}
		}
		else {
			if (!roadLine.empty() && roadLine.back()->getWatch() == -1) roadLine.pop_back();
			roadLine.push_back(new Line(5));
		}
        moving_index_x -= 1;
        moving_index_y += 1;
    }

    while (mouse_y > 0 && mouse_x > 0 && map->GetIndexValue(moving_index_x - 1, moving_index_y - 1) < 3 && mouse_x < moving_index_x && mouse_y < moving_index_y)
    {
        if (!roadLine.empty() && roadLine.back()->getWatch() == 3)
        {
            roadLine.pop_back();
			if (roadLine.size() == 0)
			{
				TRACE("empty\n");
				roadLine.push_back(new Line(-1));
			}
		}
		else {
			if (!roadLine.empty() && roadLine.back()->getWatch() == -1) roadLine.pop_back();
			roadLine.push_back(new Line(7));
		}
        moving_index_x -= 1;
        moving_index_y -= 1;
    }

    while (mouse_x < ROW && map->GetIndexValue(moving_index_x + 1, moving_index_y) < 3 && mouse_x > moving_index_x)
    {
        if (!roadLine.empty() && roadLine.back()->getWatch() == 6)
        {
            roadLine.pop_back();
			if (roadLine.size() == 0)
			{
				TRACE("empty\n");
				roadLine.push_back(new Line(-1));
			}
		}
		else {
			if (!roadLine.empty() && roadLine.back()->getWatch() == -1) roadLine.pop_back();
			roadLine.push_back(new Line(2));
		}
        moving_index_x += 1;
    }

    while (mouse_x > 0 && map->GetIndexValue(moving_index_x - 1, moving_index_y) < 3 && mouse_x < moving_index_x)
    {
        if (!roadLine.empty() && roadLine.back()->getWatch() == 2)
        {
            roadLine.pop_back();
			if (roadLine.size() == 0)
			{
				TRACE("empty\n");
				roadLine.push_back(new Line(-1));
			}
		}
		else {
			if (!roadLine.empty() && roadLine.back()->getWatch() == -1) roadLine.pop_back();
			roadLine.push_back(new Line(6));
		}
        moving_index_x -= 1;
    }

    while (mouse_y < COL && map->GetIndexValue(moving_index_x, moving_index_y + 1) < 3 && mouse_y > moving_index_y)
    {
        if (!roadLine.empty() && roadLine.back()->getWatch() == 0)
        {
            roadLine.pop_back();
			if (roadLine.size() == 0)
			{
				TRACE("empty\n");
				roadLine.push_back(new Line(-1));
			}
		}
		else {
			if (!roadLine.empty() && roadLine.back()->getWatch() == -1) roadLine.pop_back();
			roadLine.push_back(new Line(4));
		}
        moving_index_y += 1;
    }

    while (mouse_y > 0 && map->GetIndexValue(moving_index_x, moving_index_y - 1) < 3 && mouse_y < moving_index_y)
    {
        if (!roadLine.empty() && roadLine.back()->getWatch() == 4)
        {
            roadLine.pop_back();
			if (roadLine.size() == 0)
			{
				TRACE("empty\n");
				roadLine.push_back(new Line(-1));
			}
		}
		else {
			if (!roadLine.empty() && roadLine.back()->getWatch() == -1) roadLine.pop_back();
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

#pragma region Other
void Soldier::TestNext(CGameMap* map)
{
    int door_x, door_y;

    if (map->GetIndexValue(index_x - 1, index_y - 1) == 15 || map->GetIndexValue(index_x - 1, index_y - 1) == 16 || map->GetIndexValue(index_x - 1, index_y - 1) == 17 || map->GetIndexValue(index_x - 1, index_y - 1) == 18)
    {
        isNextDoor = true;
        door_x = index_x - 1;
        door_y = index_y - 1;
    }

    if (map->GetIndexValue(index_x, index_y - 1) == 15 || map->GetIndexValue(index_x, index_y - 1) == 16 || map->GetIndexValue(index_x, index_y - 1) == 17 || map->GetIndexValue(index_x, index_y - 1) == 18)
    {
        isNextDoor = true;
        door_x = index_x;
        door_y = index_y - 1;
    }

    if (map->GetIndexValue(index_x + 1, index_y - 1) == 15 || map->GetIndexValue(index_x + 1, index_y - 1) == 16 || map->GetIndexValue(index_x + 1, index_y - 1) == 17 || map->GetIndexValue(index_x + 1, index_y - 1) == 18)
    {
        isNextDoor = true;
        door_x = index_x + 1;
        door_y = index_y - 1;
    }

    if (map->GetIndexValue(index_x - 1, index_y) == 15 || map->GetIndexValue(index_x - 1, index_y) == 16 || map->GetIndexValue(index_x - 1, index_y) == 17 || map->GetIndexValue(index_x - 1, index_y) == 18)
    {
        isNextDoor = true;
        door_x = index_x - 1;
        door_y = index_y;
    }

    if (map->GetIndexValue(index_x + 1, index_y) == 15 || map->GetIndexValue(index_x + 1, index_y) == 16 || map->GetIndexValue(index_x + 1, index_y) == 17 || map->GetIndexValue(index_x + 1, index_y) == 18)
    {
        isNextDoor = true;
        door_x = index_x + 1;
        door_y = index_y;
    }

    if (map->GetIndexValue(index_x - 1, index_y + 1) == 15 || map->GetIndexValue(index_x - 1, index_y + 1) == 16 || map->GetIndexValue(index_x - 1, index_y + 1) == 17 || map->GetIndexValue(index_x - 1, index_y + 1) == 18)
    {
        isNextDoor = true;
        door_x = index_x - 1;
        door_y = index_y + 1;
    }

    if (map->GetIndexValue(index_x, index_y + 1) == 15 || map->GetIndexValue(index_x, index_y + 1) == 16 || map->GetIndexValue(index_x, index_y + 1) == 17 || map->GetIndexValue(index_x, index_y + 1) == 18)
    {
        isNextDoor = true;
        door_x = index_x;
        door_y = index_y + 1;
    }

    if (map->GetIndexValue(index_x + 1, index_y + 1) == 15 || map->GetIndexValue(index_x + 1, index_y + 1) == 16 || map->GetIndexValue(index_x + 1, index_y + 1) == 17 || map->GetIndexValue(index_x + 1, index_y + 1) == 18)
    {
        isNextDoor = true;
        door_x = index_x + 1;
        door_y = index_y + 1;
    }
}

void Soldier::TestInRedLine(CPoint point)
{
    int mouse_x = point.x / 40, mouse_y = point.y / 40;
    int line_x = index_x, line_y = index_y;

    for (vector<Line*>::iterator iter = roadLine.begin(); iter != roadLine.end(); iter++)
    {
		if (line_x == mouse_x && line_y == mouse_y) {
			isInRoadLine = true;
			//TRACE("Mouse In the RoadLine.\n");
			//TRACE("x=%d y=%d\n", line_x, line_y);
			break;
		}
        switch ((*iter)->getWatch())
        {
            case 0:
                line_y -= 1;
                break;

            case 1:
                line_x += 1;
                line_y -= 1;
                break;

            case 2:
                line_x += 1;
                break;

            case 3:
                line_x += 1;
                line_y += 1;
                break;

            case 4:
                line_y += 1;
                break;

            case 5:
                line_x -= 1;
                line_y += 1;
                break;

            case 6:
                line_x -= 1;
                break;

            case 7:
                line_x -= 1;
                line_y -= 1;
                break;

            default:
                break;
        }
		
    }
}

#pragma endregion



}