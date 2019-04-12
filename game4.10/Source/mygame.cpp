/*
 * mygame.cpp: ���ɮ��x�C��������class��implementation
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * History:
 *   2002-03-04 V3.1
 *          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStateRun.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
 *   2006-02-08 V4.2
 *      1. Revise sample screens to display in English only.
 *      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
 *      3. Rename OnInitialUpdate() -> OnInit().
 *      4. Fix the bug that OnBeginState() of GameStateInit is not called.
 *      5. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      6. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2006-12-30
 *      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
 *         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
 *   2010-03-23 V4.6
 *      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/


#include <vector>
#include "stdafx.h"
#include "Resource.h"
#include "MainFrm.h"
#include "gameDoc.h"
#include <mmsystem.h>
#include <windows.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"



namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C���}�Y�e������
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame* g)
    : CGameState(g)
{
}

void CGameStateInit::OnInit()
{
    //
    // ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
    //     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
    //
    ShowInitProgress(0);	// �@�}�l��loading�i�׬�0%
    //
    // �}�l���J���
    //
    logo.LoadBitmap(IDB_BACKGROUND);
    Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
    //
    // ��OnInit�ʧ@�|����CGameStaterRun::OnInit()�A�ҥH�i���٨S��100%
    //
}

void CGameStateInit::OnBeginState()
{
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_ESC = 27;
    const char KEY_SPACE = ' ';

    if (nChar == KEY_SPACE)
        GotoGameState(GAME_STATE_RUN);						// ������GAME_STATE_RUN
    else if (nChar == KEY_ESC)								// Demo �����C������k
        PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// �����C��
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
    GotoGameState(GAME_STATE_RUN);		// ������GAME_STATE_RUN
}

void CGameStateInit::OnShow()
{
    //
    // �K�Wlogo
    //
    logo.SetTopLeft((SIZE_X - logo.Width()) / 2, SIZE_Y / 8);
    logo.ShowBitmap();
    //
    // Demo�ù��r�����ϥΡA���L�}�o�ɽкɶq�קK�����ϥΦr���A���CMovingBitmap����n
    //
    CDC* pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC
    CFont f, *fp;
    f.CreatePointFont(160, "Times New Roman");	// ���� font f; 160���16 point���r
    fp = pDC->SelectObject(&f);					// ��� font f
    pDC->SetBkColor(RGB(0, 0, 0));
    pDC->SetTextColor(RGB(255, 255, 0));
    pDC->TextOut(120, 220, "Please click mouse or press SPACE to begin.");
    pDC->TextOut(5, 395, "Press Ctrl-F to switch in between window mode and full screen mode.");

    if (ENABLE_GAME_PAUSE)
        pDC->TextOut(5, 425, "Press Ctrl-Q to pause the Game.");

    pDC->TextOut(5, 455, "Press Alt-F4 or ESC to Quit.");
    pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
    CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
}

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����������A(Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame* g)
    : CGameState(g)
{
}

void CGameStateOver::OnMove()
{
    counter--;

    if (counter < 0)
        GotoGameState(GAME_STATE_INIT);
}

void CGameStateOver::OnBeginState()
{
    counter = 30 * 5; // 5 seconds
}

void CGameStateOver::OnInit()
{
    //
    // ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
    //     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
    //
    ShowInitProgress(66);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���66%
    //
    // �}�l���J���
    //
    Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
    //
    // �̲׶i�׬�100%
    //
    ShowInitProgress(100);
}

void CGameStateOver::OnShow()
{
    CDC* pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC
    CFont f, *fp;
    f.CreatePointFont(160, "Times New Roman");	// ���� font f; 160���16 point���r
    fp = pDC->SelectObject(&f);					// ��� font f
    pDC->SetBkColor(RGB(0, 0, 0));
    pDC->SetTextColor(RGB(255, 255, 0));
    char str[80];								// Demo �Ʀr��r�ꪺ�ഫ
    sprintf(str, "Game Over ! (%d)", counter / 30);
    pDC->TextOut(240, 210, str);
    pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
    CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
}

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
/////////////////////////////////////////////////////////////////////////////

CGameMap::CGameMap() : X(0), Y(0), MW(SIZE), MH(SIZE)
{
	random_num = 0;
	int map_init[COL][ROW] = 
	{
		{5,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,6},
		{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
		{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
		{3,0,0,0,5,4,4,4,4,4,13,4,4,4,4,4,10,4,4,4,4,4,11,4,4,4,4,6,0,0,0,3},
		{3,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,3,0,0,0,3},
		{3,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,3,0,0,0,3},
		{3,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,3,0,0,0,3},
		{3,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,12,0,0,0,0,0,0,0,0,0,0,12,0,0,0,3},
		{3,0,0,0,14,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,3,0,0,0,3},
		{3,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,3,0,0,0,3},
		{3,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,3,0,0,0,3},
		{3,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,3,0,0,0,3},
		{3,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,3,0,0,0,3},
		{3,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,3,0,0,0,3},
		{3,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,3,0,0,0,3},
		{3,0,0,0,12,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,14,0,0,0,3},
		{3,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,18,0,0,0,0,0,0,0,0,0,0,3,0,0,0,3},
		{3,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,3,0,0,0,3},
		{3,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,3,0,0,0,3},
		{3,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,3,0,0,0,3},
		{3,0,0,0,7,4,4,4,4,4,4,11,4,4,4,4,9,4,4,4,13,4,4,15,4,4,4,8,0,0,0,3},
		{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
		{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
		{7,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,8}
		
	};


    for (int i = 0; i < 24; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            map[i][j] = map_init[i][j];
        }
    }

    map[1][1] = 1;  //�X���I
}

void CGameMap::LoadBitmap()
{
    wall_Horizontal.LoadBitmap(WALL_HORIZONTAL, RGB(255, 255, 255));
	wall_Straight.LoadBitmap(WALL_STRAIGHT, RGB(255, 255, 255));
	wall_LD.LoadBitmap(WALL_LD, RGB(255, 255, 255));
	wall_LU.LoadBitmap(WALL_LU, RGB(255, 255, 255));
	wall_RD.LoadBitmap(WALL_RD, RGB(255, 255, 255));
	wall_RU.LoadBitmap(WALL_RU, RGB(255, 255, 255));
	wall_Invertr_T.LoadBitmap(WALL_INVERTE_T, RGB(255, 255, 255));
	wall_T.LoadBitmap(WALL_T, RGB(255, 255, 255));
	glass_Straight.LoadBitmap(GLASS_STR, RGB(255, 255, 255));
	glass_Horizontal.LoadBitmap(GLASS_HOR, RGB(255, 255, 255));
	glass_BrokenHorizontal.LoadBitmap(GLASS_BROKENHOR, RGB(255, 255, 255));
	glass_BrokenStraight.LoadBitmap(GLASS_BROKENSTR, RGB(255, 255, 255));
	door_HorLeft.LoadBitmap(DOOR_HOR_LEFT, RGB(255, 255, 255));
	door_HorRight.LoadBitmap(DOOR_HOR_RIGHT, RGB(255, 255, 255));
	door_StrDown.LoadBitmap(DOOR_STR_DOWN, RGB(255, 255, 255));
	door_StrUp.LoadBitmap(DOOR_STR_UP, RGB(255, 255, 255));
}
void CGameMap::OnShow()
{
    for (int i = 0; i < 32; i++)
    {
        for (int j = 0; j < 24; j++)
        {
            switch (map[j][i])
            {

                case 3:
                    wall_Straight.SetTopLeft(X + (MW * i), Y + (MH * j));
					wall_Straight.ShowBitmap();
                    break;
				case 4:
					wall_Horizontal.SetTopLeft(X + (MW * i), Y + (MH * j));
					wall_Horizontal.ShowBitmap();
					break;
				case 5:
					wall_LU.SetTopLeft(X + (MW * i), Y + (MH * j));
					wall_LU.ShowBitmap();
					break;
				case 6:
					wall_RU.SetTopLeft(X + (MW * i), Y + (MH * j));
					wall_RU.ShowBitmap();
					break;
				case 7:
					wall_LD.SetTopLeft(X + (MW * i), Y + (MH * j));
					wall_LD.ShowBitmap();
					break;
				case 8:
					wall_RD.SetTopLeft(X + (MW * i), Y + (MH * j));
					wall_RD.ShowBitmap();
					break;
				case 9:
					wall_Invertr_T.SetTopLeft(X + (MW * i), Y + (MH * j));
					wall_Invertr_T.ShowBitmap();
					break;
				case 10:
					wall_T.SetTopLeft(X + (MW * i), Y + (MH * j));
					wall_T.ShowBitmap();
					break;
				case 11:
					glass_Horizontal.SetTopLeft(X + (MW*i), Y + (MH*j));
					glass_Horizontal.ShowBitmap();
					break;
				case 12:
					glass_Straight.SetTopLeft(X + (MW*i), Y + (MH*j));
					glass_Straight.ShowBitmap();
					break;
				case 13:
					glass_BrokenHorizontal.SetTopLeft(X + (MW*i), Y + (MH*j));
					glass_BrokenHorizontal.ShowBitmap();
					break;
				case 14:
					glass_BrokenStraight.SetTopLeft(X + (MW*i), Y + (MH*j));
					glass_BrokenStraight.ShowBitmap();
					break;
				case 15:
					door_HorLeft.SetTopLeft(X + (MW*i), Y + (MH*j));
					door_HorLeft.ShowBitmap();
					break;
				case 16:
					door_HorRight.SetTopLeft(X + (MW*i), Y + (MH*j));
					door_HorRight.ShowBitmap();
					break;
				case 17:
					door_StrUp.SetTopLeft(X + (MW*i), Y + (MH*j));
					door_StrUp.ShowBitmap();
					break;
				case 18:
					door_StrDown.SetTopLeft(X + (MW*i), Y + (MH*j));
					door_StrDown.ShowBitmap();
					break;
                default:
                    break;
            }
        }
    }
	
}
int CGameMap::GetIndexValue(int x, int y)
{
    return map[x][y];
}
void CGameMap::SetIndexValue(int x, int y, int value)
{
    map[x][y] = value;
}

void CGameMap::InitializeBouncingBall(int ini_index, int row, int col) {
	const int VELOCITY = 10;
	const int BALL_PLC_HEIGHT = 15;
	int floor = Y + (row + 1)*MH - BALL_PLC_HEIGHT;

	bballs[ini_index].LoadBitmap();
	bballs[ini_index].SetFloor(floor);
	bballs[ini_index].SetVelocity(VELOCITY + col);
	bballs[ini_index].SetXY(X + col * MW + MW / 2, floor);
}

void CGameMap::RandomBouncingBall() {
	const int MAX_RAND_NUM = 10;
	random_num = (rand() % MAX_RAND_NUM) + 1;

	bballs = new CBouncingBall[random_num];
	int ini_index = 0;
	for (int row = 0; row < 4; row++) {
		for(int col = 0; col < 5; col++) {
			if (map[row][col] != 0 && ini_index < random_num) {
				InitializeBouncingBall(ini_index, row, col);
				ini_index++;
			}
		}
	}
}

void CGameMap::OnKeyDown(UINT nChar) {
	const int KEY_SPACE = 0x20;
	/*if (nChar == KEY_SPACE)
		RandomBouncingBall();*/
}

void CGameMap::OnMove() {
	for (int i = 0; i < random_num; i++) {
		bballs[i].OnMove();
	}
}

CGameMap::~CGameMap() {}


CGameStateRun::CGameStateRun(CGame* g)
    : CGameState(g), NUMBALLS(28)
{
    ball = new CBall [NUMBALLS];
}

CGameStateRun::~CGameStateRun()
{
    delete [] ball;
}

void CGameStateRun::OnBeginState()
{
    const int BALL_GAP = 90;
    const int BALL_XY_OFFSET = 45;
    const int BALL_PER_ROW = 7;
    const int HITS_LEFT = 10;
    const int HITS_LEFT_X = 590;
    const int HITS_LEFT_Y = 0;
    const int BACKGROUND_X = 60;
    const int ANIMATION_SPEED = 15;

    for (int i = 0; i < NUMBALLS; i++)  				// �]�w�y���_�l�y��
    {
        int x_pos = i % BALL_PER_ROW;
        int y_pos = i / BALL_PER_ROW;
        ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
        ball[i].SetDelay(x_pos);
        ball[i].SetIsAlive(true);
    }

    people.Initialize();
    background.SetTopLeft(BACKGROUND_X, 0);				// �]�w�I�����_�l�y��
    help.SetTopLeft(0, SIZE_Y - help.Height());			// �]�w�����Ϫ��_�l�y��
    hits_left.SetInteger(HITS_LEFT);					// ���w�ѤU��������
    hits_left.SetTopLeft(HITS_LEFT_X, HITS_LEFT_Y);		// ���w�ѤU�����ƪ��y��
    //CAudio::Instance()->Play(AUDIO_NTUT, true);			// ���� MIDI
}

void CGameStateRun::OnMove()							// ���ʹC������
{
	//
	// �p�G�Ʊ�ק�cursor���˦��A�h�N�U���{����commment�����Y�i
	//
	// SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
	//
	// ���ʭI���Ϫ��y��
	//
	//
	// ���ʲy
	//
	vector<int> ptr = people.GetRoadLine();
	if (!ptr.empty() && !people.IsChoosen()) {
		TRACE("%d\n", people.GetIt());
		switch (people.GetIt())
		{
		case 0:
			people.SetMovingUp(true);
			break;
		case 1:
			people.SetMovingRightUp(true);
			break;
		case 2:
			people.SetMovingRight(true);
			break;
		case 3:
			people.SetMovingRightDown(true);
			break;
		case 4:
			people.SetMovingDown(true);
			break;
		case 5:
			people.SetMovingLeftDown(true);
			break;
		case 6:
			people.SetMovingLeft(true);
			break;
		case 7:
			people.SetMovingLeftUp(true);
			break;
		default:
			break;
		}
		people.OnMove();

		people.SetMovingUp(false);
		people.SetMovingRight(false);
		people.SetMovingDown(false);
		people.SetMovingLeft(false);
		people.SetMovingRightUp(false);
		people.SetMovingRightDown(false);
		people.SetMovingLeftUp(false);
		people.SetMovingLeftDown(false);
	}
	TRACE("%d,%d\n", people.GetIndexX(), people.GetIndexY());
    //
    // �������l
    //
	map.OnMove();
}

void CGameStateRun::OnInit()  								// �C������Ȥιϧγ]�w
{
    //
    // ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
    //     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
    //
    ShowInitProgress(33);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���33%
    //
    // �}�l���J���
    //
    people.LoadBitmap();
    background.LoadBitmap(IDB_BACKGROUND);					// ���J�I�����ϧ�
    map.LoadBitmap();
    back.LoadBitmap("Bitmaps/back.bmp");
    //
    // ��������Loading�ʧ@�A�����i��
    //
    ShowInitProgress(50);
    Sleep(300); // ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
    //
    // �~����J��L���
    //
    help.LoadBitmap(IDB_HELP, RGB(255, 255, 255));				// ���J�������ϧ�
    corner.LoadBitmap(IDB_CORNER);							// ���J�����ϧ�
    corner.ShowBitmap(background);							// �Ncorner�K��background
    bball.LoadBitmap();										// ���J�ϧ�
    hits_left.LoadBitmap();
    //CAudio::Instance()->Load(AUDIO_NTUT,  "sounds\\song.mid");	// ���J�s��2���n��ntut.mid
    //
    // ��OnInit�ʧ@�|����CGameStaterOver::OnInit()�A�ҥH�i���٨S��100%
    //
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT = 0x41; // keyboard�V����
	const char KEY_UP = 0x57; // keyboard�V�W��
	const char KEY_RIGHT = 0x44; // keyboard�V�k��
	const char KEY_DOWN = 0x53; // keyboard�V�U��
	map.OnKeyDown(nChar);
	if (nChar == KEY_LEFT)
	{
		people.SetWatchLeft(true);
	}

	if (nChar == KEY_RIGHT)
	{
		people.SetWatchRight(true);
	}

	if (nChar == KEY_UP)
	{
		people.SetWatchUp(true);
	}

	if (nChar == KEY_DOWN)
	{
		people.SetWatchDown(true);
	}

}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT = 0x41; // keyboard�V����
	const char KEY_UP = 0x57; // keyboard�V�W��
	const char KEY_RIGHT = 0x44; // keyboard�V�k��
	const char KEY_DOWN = 0x53; // keyboard�V�U��	

	if (nChar == KEY_LEFT)
		people.SetWatchLeft(false);

	if (nChar == KEY_RIGHT)
		people.SetWatchRight(false);

	if (nChar == KEY_UP)
		people.SetWatchUp(false);

	if (nChar == KEY_DOWN)
		people.SetWatchDown(false);
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
{
	TRACE("Mouse left button down\n");
	if (point.x>=people.GetX1()&&point.x<=people.GetX2()&&point.y>=people.GetY1()&&point.y<=people.GetY2())
	{
		TRACE("Get choosen\n");
		people.SetChoosen(true);
	}
	
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
	TRACE("Mouse left button up\n");
	people.SetChoosen(false);
	//if()
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{	
	static int mouse_x, mouse_y;
	
	mouse_x = point.x / SIZE;
	mouse_y = point.y / SIZE;
	TRACE("mouse position in array: %d %d\n", mouse_x, mouse_y);
	//TRACE("%d,%d\n", point.x, point.y);
	if (people.IsChoosen())
	{
		people.SetRoadLine(mouse_x, mouse_y,map);
	}
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
{
    people.SetMovingRight(true);
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
    people.SetMovingRight(false);
	
}

void CGameStateRun::GetMouse(UINT nFlags, CPoint point)
{

}

void CGameStateRun::OnShow()
{
    //
    //  �`�N�GShow�̭��d�U���n���ʥ��󪫥󪺮y�СA���ʮy�Ъ��u�@����Move���~��A
    //        �_�h��������sø�Ϯ�(OnDraw)�A����N�|���ʡA�ݰ_�ӷ|�ܩǡC���ӳN�y
    //        ���AMove�t�dMVC����Model�AShow�t�dView�A��View�������Model�C
    //
    //
    //  �K�W�I���ϡB�����ơB�y�B���l�B�u�����y
    //
    help.ShowBitmap();					// �K�W������
    back.ShowBitmap();
    map.OnShow();
	SetCursor(LoadCursor(NULL, IDC_CROSS));
	people.OnShow();
    corner.SetTopLeft(0, 0);
    corner.ShowBitmap();
    corner.SetTopLeft(SIZE_X - corner.Width(), SIZE_Y - corner.Height());
    corner.ShowBitmap();
	
}
}