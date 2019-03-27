#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include <vector>
#include "mygame.h"
#include "CEraser.h"
#include<iostream>

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// CEraser: Eraser class
/////////////////////////////////////////////////////////////////////////////

CEraser::CEraser()
{
    Initialize();
}

int CEraser::GetX1()
{
    return x;
}

int CEraser::GetY1()
{
    return y;
}

int CEraser::GetX2()
{
    return x + animation.Width();
}

int CEraser::GetY2()
{
    return y + animation.Height();
}

void CEraser::Initialize()
{
    const int X_POS = 20;
    const int Y_POS = 20;
    x = X_POS;
    y = Y_POS;
    index_x = index_y = 1;
    isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
    it = roadLine.end();
}

void CEraser::LoadBitmap()
{
    animation2.AddBitmap(IDB_PEOPLE2, RGB(255, 255, 255));
    animation.AddBitmap(IDB_PEOPLE, RGB(255, 255, 255));
}

void CEraser::OnMove()
{
    const int STEP_SIZE = 4;
    animation.OnMove();
    //int iter_x, iter_y;


	if (isMovingLeft)
	{
		if(x > (index_x-1) * 20)
			x -= STEP_SIZE;
		else
		{
			index_x--;
			it++;
		}
	}
       

	if (isMovingRight)
	{
		if (x < (index_x+1) * 20)
			x += STEP_SIZE;
		else
		{
			index_x++;
			it++;
		}
	}

	if (isMovingUp)
	{
		if (y > (index_y- 1) * 20)
			y -= STEP_SIZE;
		else
		{
			index_y--;
			it++;
		}
	}

    if (isMovingDown)
	{
		if (y < (index_y + 1) * 20)
			y += STEP_SIZE;
		else
		{
			index_y++;
			it++;
		}
	}

	if (isMovingRightUp)
	{
		if (y > (index_y - 1) * 20 && x < (index_x + 1) * 20)
		{
			y -= STEP_SIZE;
			x += STEP_SIZE;
		}
			
		else
		{
			index_y--;
			index_x++;
			it++;
		}
	}
	if (isMovingRightDown)
	{
		if (y < (index_y + 1) * 20 && x < (index_x + 1) * 20)
		{
			y += STEP_SIZE;
			x += STEP_SIZE;
		}

		else
		{
			index_y++;
			index_x++;
			it++;
		}
	}
	if (isMovingLeftUp)
	{
		if (y > (index_y - 1) * 20 && x > (index_x - 1) * 20)
		{
			y -= STEP_SIZE;
			x -= STEP_SIZE;
		}

		else
		{
			index_y--;
			index_x--;
			it++;
		}
	}
	if (isMovingLeftDown)
	{
		if (y < (index_y + 1) * 20 && x > (index_x - 1) * 20)
		{
			y += STEP_SIZE;
			x -= STEP_SIZE;
		}

		else
		{
			index_y++;
			index_x--;
			it++;
		}
	}

	if (it == roadLine.end())
	{
		roadLine.clear();
		roadLine.resize(0);
		it = roadLine.end();
	}

    //x:({x}), y:({y}),index_x:({index_x}),index_y({index_y})
}
int CEraser::GetIndexX()
{
    return index_x;
}
int CEraser::GetIndexY()
{
    return index_y;
}
void CEraser::MoveLeftIndex()
{
    index_x -= 1;
}
void CEraser::MoveRightIndex()
{
    index_x += 1;
}
void CEraser::MoveUpIndex()
{
    index_y -= 1;
}
void CEraser::MoveDownIndex()
{
    index_y += 1;
}
void CEraser::SetMovingRightDown(bool flag)
{
	isMovingRightDown = flag;
}
void CEraser::SetMovingLeftDown(bool flag)
{
	isMovingLeftDown = flag;
}
void CEraser::SetMovingRightUp(bool flag)
{
	isMovingRightUp = flag;
}
void CEraser::SetMovingLeftUp(bool flag)
{
	isMovingLeftUp = flag;
}
void CEraser::SetMovingDown(bool flag)
{
    isMovingDown = flag;
}

void CEraser::SetMovingLeft(bool flag)
{
    isMovingLeft = flag;
}

void CEraser::SetMovingRight(bool flag)
{
    isMovingRight = flag;
}

void CEraser::SetMovingUp(bool flag)
{
    isMovingUp = flag;
}

void CEraser::SetChoosen(bool flag)
{
    isChoosen = flag;
}

void CEraser::SetXY(int nx, int ny)
{
    x = nx;
    y = ny;
}
void CEraser::OnShow()
{
    animation.SetTopLeft(x, y);
    animation.OnShow();
}
bool CEraser::IsChoosen()
{
    return isChoosen;
}
void CEraser::SetRoadLine(int mouse_x, int mouse_y, CGameMap& map)
{
    if (roadLine.empty())
    {
        moving_index_x = index_x;
        moving_index_y = index_y;
    }
	TRACE("Mouse %d %d\n", mouse_x, mouse_y);
	TRACE("moving_index position in array: %d %d\n", moving_index_x, moving_index_y);
    while (mouse_y > 0 && mouse_x < ROW && map.GetIndexValue(moving_index_y - 1, moving_index_x + 1) != 1 && mouse_x  > moving_index_x && mouse_y <  moving_index_y)
    {
        roadLine.push_back(1);
        moving_index_x += 1;
        moving_index_y -= 1;
    }

    while (mouse_y < COL &&mouse_x < ROW &&map.GetIndexValue(moving_index_y + 1, moving_index_x + 1) != 1 && mouse_x >  moving_index_x && mouse_y >  moving_index_y)
    {
        roadLine.push_back(3);
        moving_index_x += 1;
        moving_index_y += 1;
    }

    while (mouse_y  < COL &&mouse_x > 0 &&map.GetIndexValue(moving_index_y + 1, moving_index_x - 1) != 1 && mouse_x <  moving_index_x && mouse_y >  moving_index_y)
    {
        roadLine.push_back(5);
        moving_index_x -= 1;
        moving_index_y += 1;
    }

    while (mouse_y > 0 && mouse_x > 0 && map.GetIndexValue(moving_index_y - 1, moving_index_x - 1) != 1 && mouse_x < moving_index_x && mouse_y < moving_index_y)
    {
        roadLine.push_back(7);
        moving_index_x -= 1;
        moving_index_y -= 1;
    }

    while (mouse_x < ROW &&map.GetIndexValue(moving_index_y, moving_index_x+1) != 1 && mouse_x > moving_index_x)
    {
        roadLine.push_back(2);
        moving_index_x += 1;
    }

    while (mouse_x > 0 && map.GetIndexValue(moving_index_y, moving_index_x-1) != 1 && mouse_x  < moving_index_x)
    {
        roadLine.push_back(6);
        moving_index_x -= 1;
    }

    while (mouse_y < COL && map.GetIndexValue(moving_index_y+1, moving_index_x) != 1 && mouse_y  > moving_index_y)
    {
        roadLine.push_back(4);
        moving_index_y += 1;
    }

    while (mouse_y > 0 && map.GetIndexValue(moving_index_y-1, moving_index_x) != 1 && mouse_y  < moving_index_y)
    {
        roadLine.push_back(0);
        moving_index_y -= 1;
    }

    it = roadLine.begin();
}
const vector<int>& CEraser::GetRoadLine()
{
    return roadLine;
}

const int  CEraser::GetIt()
{
    if (it != roadLine.end())
    {
        return *it;
    }

    return -1;
}
}