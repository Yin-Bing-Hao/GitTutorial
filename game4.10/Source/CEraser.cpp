#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include <vector>
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
    isChoosen=isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
}

void CEraser::LoadBitmap()
{
	animation2.AddBitmap(IDB_PEOPLE2, RGB(255, 255, 255));
    animation.AddBitmap(IDB_PEOPLE, RGB(255, 255, 255));
}

void CEraser::OnMove()
{
    const int STEP_SIZE = 4;
    int count = 0;
    animation.OnMove();
	

    if (isMovingLeft && x >= index_x * 20)
        x -= STEP_SIZE;

<<<<<<< HEAD
    if (this->GetX2() <= (index_x + 1) * 20)
    {
=======
    if (isMovingRight && this->GetX2() <= (index_x + 1) * 20)
>>>>>>> c8bd7f0bdaa1728f5c51dab00eaf9311ddd2ed98
        x += STEP_SIZE;

    if (isMovingUp && y >= index_y * 20)
        y -= STEP_SIZE;

    if (isMovingDown && this->GetY2() <= (index_y + 1) * 20)
        y += STEP_SIZE;
<<<<<<< HEAD
    }
=======
>>>>>>> c8bd7f0bdaa1728f5c51dab00eaf9311ddd2ed98

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

void CEraser::SetPeopleChioce(bool flag)
{
	isChoice = flag;
}

void CEraser::SetXY(int nx, int ny)
{
    x = nx;
    y = ny;
}

bool CEraser::isPeopleChoice()
{
	if (isChoice)
		return true;
	else
		return false;
}

void CEraser::OnShow()
{
	if (isChoice) {
		animation2.SetTopLeft(x, y);
		animation2.OnShow();
	}
	else {
		animation.SetTopLeft(x, y);
		animation.OnShow();
	}
}
void CEraser::SetRoadLine(int mouse_x, int mouse_y)
{
	TRACE("mouse %d %d\n", mouse_x, mouse_y);
	if (roadLine.empty())	
	{
		moving_index_x = index_x;
		moving_index_y = index_y;
	}
	while (mouse_x > moving_index_x && mouse_y > moving_index_y)
	{
		roadLine.push_back(3);
		moving_index_x += 1;
		moving_index_y += 1;
	}
	while(mouse_x > moving_index_x && mouse_y < moving_index_y)
	{
		roadLine.push_back(1);
		moving_index_x += 1;
		moving_index_y -= 1;
	}
	while (mouse_x < moving_index_x && mouse_y < moving_index_y)
	{
		roadLine.push_back(7);
		moving_index_x -= 1;
		moving_index_y -= 1;
	}
	while(mouse_x < moving_index_x && mouse_y > moving_index_y)
	{
		roadLine.push_back(5);
		moving_index_x -= 1;
		moving_index_y += 1;
	}
	while (mouse_x < moving_index_x)
	{
		roadLine.push_back(6);
		moving_index_x -= 1;
	}
	while (mouse_x > moving_index_x)
	{
		roadLine.push_back(2);
		moving_index_x += 1;

	}
	while (mouse_y > moving_index_y)
	{
		roadLine.push_back(4);
		moving_index_y += 1;
	}
	while (mouse_y < moving_index_y)
	{
		roadLine.push_back(0);
		moving_index_y -= 1;
	}
}
void CEraser::SetChoosen(bool choosen)
{
	isChoosen = choosen;
}
bool CEraser::GetChoosen()
{
	return isChoosen;
}
void CEraser::ShowRoadLine()
{
	//TRACE("RoadLine[");
	for (vector<int>::iterator iter = roadLine.begin();iter != roadLine.end();iter++)
	{
		TRACE("%d,\n ", *iter);
	}
	//TRACE("]\n");
	}
}