/*
 * mygame.cpp: 本檔案儲遊戲本身的class的implementation
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
 *      Rewrite the codes for CBall and Soldier.
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
#include <cmath>
#include "Weapons.h"
#include "Back.h"
#include "mygame.h"
#include <thread>
#include<mutex>
#include<memory>

using namespace std;
namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
/////////////////////////////////////////////////////////////////////////////

	static bool init_audio_play = false;
	static bool run_init = false;
	static bool run_init_load = true;
	static bool enemy_all_die = false;
	static bool no_injury = true;
	static bool new_game = true;
	static bool in_time = true;
	static bool fail = false;
CGameStateInit::CGameStateInit(CGame* g)
    : CGameState(g)
{
}

void CGameStateInit::OnInit()
{
    //
    // 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
    //     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
    //
    ShowInitProgress(0);	// 一開始的loading進度為0%
    //
    // 開始載入資料
    //
	CAudio::Instance()->Load(AUDIO_INIT_BACKGRUOND, "Sounds\\background_sound.mp3");
	background.LoadBitmap("Bitmaps/init_background.bmp");
    Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
    //
    // 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
    //
}

void CGameStateInit::OnBeginState()
{
	
}
void CGameStateInit::OnMove() {
	if (!init_audio_play)
	{
		CAudio::Instance()->Play(AUDIO_INIT_BACKGRUOND, true);
		init_audio_play = true;
	}
}
void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_ESC = 27;
    const char KEY_SPACE = ' ';

    if (nChar == KEY_SPACE)
        GotoGameState(GAME_STATE_RUN);						// 切換至GAME_STATE_RUN
    else if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
        PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (point.x >= 65 && point.x <= 220 && point.y >= 649 && point.y <= 699)
	{
		CAudio::Instance()->Stop(AUDIO_INIT_BACKGRUOND);
		init_audio_play = false;
		GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
	}
	if (point.x >= 65 && point.x <= 220 && point.y >= 792 && point.y <= 842)
	{
		PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
	}
	
}

void CGameStateInit::OnShow()
{
	background.SetTopLeft(0, 0);
	background.ShowBitmap();
}

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame* g)
    : CGameState(g)
{
}

void CGameStateOver::OnBeginState()
{
	CAudio::Instance()->Stop(AUDIO_FUCKYEA);
	if (enemy_all_die) {
		CAudio::Instance()->Play(AUDIO_MISSION_COMPLETE, false);
		
	}
	else if (fail) {
		CAudio::Instance()->Play(AUDIO_MISSION_FAIL, false);
		
	}
}

void CGameStateOver::OnInit()
{
    //
    // 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
    //     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
    //
    ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
    //
    // 開始載入資料
    //
	One_Star.LoadBitmap("Bitmaps/One_Star.bmp", RGB(255, 255, 255));
	Two_Star.LoadBitmap("Bitmaps/Two_Star.bmp", RGB(255, 255, 255));
	Three_Star.LoadBitmap("Bitmaps/Three_Star.bmp", RGB(255, 255, 255));
	Fail.LoadBitmap("Bitmaps/FAIL.bmp", RGB(255, 255, 255));
	CAudio::Instance()->Load(AUDIO_MISSION_COMPLETE, "Sounds\\mission_complete.mp3");
	CAudio::Instance()->Load(AUDIO_MISSION_FAIL, "Sounds\\mission_fail.mp3");
    Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
    //
    // 最終進度為100%
    //
    ShowInitProgress(100);
}

void CGameStateOver::OnMove()
{
	
}

void CGameStateOver::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (enemy_all_die) 
	{
		if (point.x > 690 && point.x < 825 && point.y>600 && point.y < 650) {
			CAudio::Instance()->Stop(AUDIO_MISSION_COMPLETE);
			GotoGameState(GAME_STATE_RUN);
		}
		else if (point.x > 460 && point.x < 600 && point.y>600 && point.y < 650) {
			CAudio::Instance()->Stop(AUDIO_MISSION_COMPLETE);
			GotoGameState(GAME_STATE_INIT);
		}
	}
	else if (fail) 
	{
		TRACE("%d %d\n", point.x, point.y);
		if (point.x > 570 && point.x < 705 && point.y>530 && point.y < 580) 
		{
			CAudio::Instance()->Stop(AUDIO_MISSION_FAIL);
			GotoGameState(GAME_STATE_RUN);
		}
	}
}

void CGameStateOver::OnShow()
{
    

	if (enemy_all_die && in_time && no_injury) {
		Three_Star.ShowBitmap();
	}
	else if (enemy_all_die && in_time || enemy_all_die && no_injury) {
		Two_Star.ShowBitmap();
	}
	else if (enemy_all_die) {
		One_Star.ShowBitmap();
	}
	else if (fail) {
		Fail.ShowBitmap();
		CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
		CFont f, *fp;
		f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
		fp = pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(RGB(24, 23, 21));
		pDC->SetTextColor(RGB(255, 255, 0));
		char str[80];								// Demo 數字對字串的轉換
		sprintf(str, "Mission Fail!!");
		pDC->TextOut(570, 365, str);
		pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC*/
	}
	
}

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////
#pragma region CGameMap
CGameMap::CGameMap() : X(0), Y(0), MW(SIZE), MH(SIZE)
{
    int map_init[COL][ROW] =
    {
        {0, 0, 7, 6, 6, 6, 12, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 12, 6, 6, 6, 6, 6, 6, 6, 12, 6, 12, 6, 6, 6, 8},
        {0, 0, 5, 0, 0, 0, 5, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 3, 3, 0, 0, 0, 5, 0, 5, 3, 0, 3, 5},
        {0, 0, 5, 0, 0, 0, 5, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 5, 0, 5, 3, 0, 3, 5},
        {0, 0, 5, 0, 0, 0, 5, 3, 0, 0, 3, 0, 0, 3, 0, 0, 0, 5, 0, 0, 0, 0, 0, 3, 3, 5, 0, 5, 3, 0, 3, 5},
        {0, 0, 5, 0, 0, 0, 5, 0, 0, 3, 3, 3, 3, 3, 3, 0, 3, 5, 0, 0, 0, 0, 0, 3, 0, 5, 0, 5, 0, 0, 0, 5},
        {0, 0, 5, 0, 0, 0, 5, 3, 3, 3, 0, 0, 0, 0, 0, 0, 3, 5, 3, 3, 3, 0, 0, 0, 0, 5, 0, 5, 0, 0, 3, 5},
        {0, 0, 5, 0, 0, 0, 5, 0, 3, 0, 0, 0, 0, 0, 0, 0, 3, 13, 6, 6, 6, 6, 6, 6, 6, 10, 0, 5, 0, 0, 3, 5},
        {0, 0, 0, 0, 0, 0, 9, 6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 3, 5},
        {0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 5},
        {7, 6, 11, 0, 0, 6, 6, 6, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 3, 0, 3, 5},
        {5, 3, 3, 0, 0, 0, 3, 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 6, 0, 6, 14},
        {5, 0, 0, 0, 0, 0, 3, 3, 13, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 6, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
        {5, 0, 0, 0, 0, 0, 3, 3, 5, 3, 3, 0, 0, 3, 0, 3, 0, 0, 3, 0, 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
        {5, 0, 0, 3, 3, 0, 0, 0, 5, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 6, 6, 6, 6, 6, 6, 6, 6, 6, 14},
        {5, 0, 3, 3, 3, 3, 0, 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 3, 3, 3, 3, 3, 0, 5},
        {5, 0, 0, 3, 3, 0, 0, 3, 5, 0, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
        {5, 0, 0, 0, 0, 0, 0, 3, 5, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 5},
        {5, 3, 3, 3, 0, 0, 0, 0, 5, 0, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 0, 5, 0, 0, 0, 3, 3, 3, 3, 0, 3, 5},
        {5, 3, 3, 3, 3, 0, 0, 3, 5, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 5, 0, 0, 0, 0, 3, 3, 0, 0, 0, 5},
        {5, 3, 3, 3, 0, 0, 0, 3, 5, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 5, 3, 3, 3, 0, 0, 0, 0, 0, 0, 5},
        {5, 0, 0, 0, 0, 3, 0, 3, 5, 3, 0, 0, 3, 3, 0, 0, 0, 0, 0, 3, 3, 5, 3, 3, 3, 3, 3, 0, 3, 3, 3, 5},
        {9, 6, 6, 6, 6, 6, 6, 6, 11, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 11, 6, 6, 6, 6, 6, 6, 6, 6, 6, 10},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2}

    };
    const int back_init[48][64] =
    {
        {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},//0
        {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//1
        {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//2
        {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//3
        {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//4
        {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//5
        {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//6
        {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//7
        {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//8
        {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//9
        {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//10
        {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//11
        {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//12
        {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//13
        {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//14
        {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//15
        {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//16
        {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//17
        {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//18
        {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//19
        {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//20
        {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3},//21
        {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3}, //22
        {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3},//23
        {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3},//24
        {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3},//25
        {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3},//26
        {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3},//27
        {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3},//28
        {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3},//29
        {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3}, //30
        {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3},//32
        {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3},//32
        {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3},//33
        {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3},//34
        {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3},//35
        {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3},//36
        {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3},//37
        {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3},//38
        {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3},//39
        {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3},//40
        {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3},//42
        {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3},//42
        {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},//43
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };
    Background = new Back*[48];

    for (int i = 0; i < 48; i++)
        Background[i] = new Back[64];

    for (int i = 0; i < 24; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            map[i][j] = map_init[i][j];
        }
    }

    for (int i = 0; i < 48; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            Background[i][j] = Back(back_init[i][j], j * 20, i * 20);
            Background[i][j].SetLight(false);
        }
    }

    //map[1][1] = 1;  //出生點
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
	wall_RT.LoadBitmap(WALL_RIGHT_T, RGB(255, 255, 255));
	wall_LT.LoadBitmap(WALL_LEFT_T, RGB(255, 255, 255));
    glass_Straight.LoadBitmap(GLASS_STR, RGB(255, 255, 255));
    glass_Horizontal.LoadBitmap(GLASS_HOR, RGB(255, 255, 255));
    glass_BrokenHorizontal.LoadBitmap(GLASS_BROKENHOR, RGB(255, 255, 255));
    glass_BrokenStraight.LoadBitmap(GLASS_BROKENSTR, RGB(255, 255, 255));
    door_HorLeft.LoadBitmap(DOOR_HOR_LEFT, RGB(255, 255, 255));
    door_HorRight.LoadBitmap(DOOR_HOR_RIGHT, RGB(255, 255, 255));
    door_StrDown.LoadBitmap(DOOR_STR_DOWN, RGB(255, 255, 255));
    door_StrUp.LoadBitmap(DOOR_STR_UP, RGB(255, 255, 255));

    for (int i = 0; i < 48; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            Background[i][j].LoadBitmap();
        }
    }
}
void CGameMap::OnMove()
{

}
void CGameMap::OnShow()
{
    for (int i = 0; i < 48; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            Background[i][j].OnShow();
        }
    }

    for (int i = 0; i < 32; i++)
    {
        for (int j = 0; j < 24; j++)
        {
            switch (map[j][i])
            {

			

				case 4:
					glass_BrokenStraight.SetTopLeft(X + (MW * i), Y + (MH * j));
					glass_BrokenStraight.ShowBitmap();
					break;
                case 5:
                    wall_Straight.SetTopLeft(X + (MW * i), Y + (MH * j));
                    wall_Straight.ShowBitmap();
                    break;

                case 6:
                    wall_Horizontal.SetTopLeft(X + (MW * i), Y + (MH * j));
                    wall_Horizontal.ShowBitmap();
                    break;

                case 7:
                    wall_LU.SetTopLeft(X + (MW * i), Y + (MH * j));
                    wall_LU.ShowBitmap();
                    break;

                case 8:
                    wall_RU.SetTopLeft(X + (MW * i), Y + (MH * j));
                    wall_RU.ShowBitmap();
                    break;

                case 9:
                    wall_LD.SetTopLeft(X + (MW * i), Y + (MH * j));
                    wall_LD.ShowBitmap();
                    break;

                case 10:
                    wall_RD.SetTopLeft(X + (MW * i), Y + (MH * j));
                    wall_RD.ShowBitmap();
                    break;

                case 11:
                    wall_Invertr_T.SetTopLeft(X + (MW * i), Y + (MH * j));
                    wall_Invertr_T.ShowBitmap();
                    break;

                case 12:
                    wall_T.SetTopLeft(X + (MW * i), Y + (MH * j));
                    wall_T.ShowBitmap();
                    break;

				case 13:
					wall_RT.SetTopLeft(X + (MW * i), Y + (MH * j));
					wall_RT.ShowBitmap();
					break;

				case 14:
					wall_LT.SetTopLeft(X + (MW * i), Y + (MH * j));
					wall_LT.ShowBitmap();
					break;

                case 15:
                    glass_Horizontal.SetTopLeft(X + (MW * i), Y + (MH * j));
                    glass_Horizontal.ShowBitmap();
                    break;

                case 16:
                    glass_Straight.SetTopLeft(X + (MW * i), Y + (MH * j));
                    glass_Straight.ShowBitmap();
                    break;

                default:
                    break;
            }
        }
    }
}
int CGameMap::GetIndexValue(int x, int y)
{
    return map[y][x];
}
void CGameMap::SetIndexValue(int x, int y, int value)
{
    map[y][x] = value;
}
void CGameMap::OnKeyDown(UINT nChar)
{
    const int KEY_SPACE = 0x20;
}

Back CGameMap::GetBackGround()
{
    return **Background;
}

void CGameMap::SetBackLight(int y, int x, bool flag)
{
    Background[y][x].SetLight(flag);
}
CGameMap::~CGameMap() 
{
	TRACE("Background size:%d\n", sizeof(Background[0][0]));
	for (int i = 0; i < 48; i++)
	{
		delete[] Background[i];
	}
	delete[] Background;
	Background = NULL;
	TRACE("Background size:%d\n", sizeof(*Background));
}
#pragma endregion
CGamePauseButton::CGamePauseButton() : X(1200), Y(880) 
{
	isSoviet = false;
	isChoose = false;
	isPause = false;
}
void CGamePauseButton::LoadBitmap()
{
	soviet.LoadBitmap("Bitmaps/Soviet.bmp");
    stop.LoadBitmap(GAME_STOP, RGB(255, 255, 255));
    start.LoadBitmap(GAME_START, RGB(255, 255, 255));
}
void CGamePauseButton::OnShow()
{
	if (isSoviet)
	{
		soviet.SetTopLeft(X, Y);
		soviet.ShowBitmap();
		isPause = false;
	}
	else
	{
		if (!isPause)
		{
			stop.SetTopLeft(X, Y);
			stop.ShowBitmap();
		}
		else
		{
			start.SetTopLeft(X, Y);
			start.ShowBitmap();
		}
	}
}
void CGamePauseButton::SetSoviet(bool flag)
{
	isSoviet = flag;
}
bool CGamePauseButton::GetSoviet()
{
	return isSoviet;
}
void CGamePauseButton::SetPause(bool flag)
{
    isPause = flag;
}
bool CGamePauseButton::GetPause()
{
    return isPause;
}
void CGamePauseButton::SetChoosen(bool flag)
{
    isChoose = flag;
}
bool CGamePauseButton::isChoosen()
{
    return isChoose;
}
CGamePauseButton::~CGamePauseButton() {}

mutex Search_mutex;

CGameStateRun::CGameStateRun(CGame* g)
    : CGameState(g)
{
}
void CGameStateRun::Clear()
{
	for (vector<Enemy*>::iterator iter = enemy.begin();iter != enemy.end();iter++)
	{
		delete (*iter);
	}
	for (vector<Furniture*>::iterator iter = furniture.begin(); iter != furniture.end(); iter++)
	{
		delete(*iter);
	}
	for (vector<Soldier*>::iterator iter = player.begin();iter != player.end();iter++)
	{
		delete (*iter);
	}
	enemy.clear();
	furniture.clear();
	player.clear();
}
CGameStateRun::~CGameStateRun()
{
	Clear();
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
	counter = 30 * 300; // 5 seconds 
	new_game = true;
    background.SetTopLeft(BACKGROUND_X, 0);				// 設定背景的起始座標
    help.SetTopLeft(0, SIZE_Y - help.Height());			// 設定說明圖的起始座標
	pause.SetSoviet(false);
	if (run_init)
	{
		OnInit();
		run_init = false;
	}
}
void CGameStateRun::Search()
{
    Search_mutex.lock();
    for (int i = 0; i < 48; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            map.SetBackLight(i, j, false);
        }
    }
	for (vector<Furniture*>::iterator iter = furniture.begin(); iter != furniture.end(); iter++)
	{
		(*iter)->SetIsSaw(false);
	}
	for (vector<Enemy*>::iterator iter = enemy.begin(); iter != enemy.end(); iter++)
	{
		(*iter)->SetIsSaw(false|| pause.GetSoviet());
		unique_ptr<thread> search_player(new thread(&Enemy::searchEnemy, (*iter), &map, player));
		if (search_player->joinable()) search_player->join();
	}
	for (vector<Soldier*>::iterator iter = player.begin();iter != player.end();iter++)
	{
		unique_ptr<thread> search_enemy(new thread(&Soldier::searchEnemy, (*iter), &map, enemy, furniture));
		if (search_enemy->joinable()) search_enemy->join();
	}
	Search_mutex.unlock();
}
void CGameStateRun::OnMove()							// 移動遊戲元素
{
    
	if (new_game) {
		CAudio::Instance()->Play(AUDIO_NEWGAME_1, false);
		CAudio::Instance()->Play(AUDIO_NEWGAME_2, false);
		CAudio::Instance()->Play(AUDIO_NEWGAME_3, false);
		CAudio::Instance()->Play(AUDIO_NEWGAME_4, false);
		CAudio::Instance()->Play(AUDIO_OK, false);
		new_game = false;
	}

	counter--;
	if (counter < 0) {
		Clear();
		fail = true;
		run_init = true;
		GotoGameState(GAME_STATE_OVER);
	}
    //
    // 如果希望修改cursor的樣式，則將下面程式的commment取消即可
    //
     SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
    //
    // 移動背景圖的座標
    //
    //
    // 移動球
    //
    //TRACE("enemy:%d\n", map.GetIndexValue(1, 5));
    if (!pause.GetPause())
    {
		static int search_count = 0;
		for (vector<Soldier*>::iterator iter = player.begin();iter != player.end();iter++)
		{
			if ((*iter) != NULL) {
				if ((*iter)->GetLifePoint() <= 0)
				{
					map.SetIndexValue((*iter)->GetIndexX(), (*iter)->GetIndexY(), 0);
					delete (*iter);
					(*iter) = NULL;
					player.erase(iter);
					no_injury = false;
					if (player.empty())
					{
						break;
					}
					else
					{
						iter = player.begin();
					}
				}
				else {
					vector<Line*> ptr = (*iter)->GetRoadLine();
					(*iter)->attackEnemy();
					if (map.GetIndexValue((*iter)->GetIndexX(), (*iter)->GetIndexY()) == 0)map.SetIndexValue((*iter)->GetIndexX(), (*iter)->GetIndexY(), 1);
					if (!ptr.empty())
					{
						switch ((*iter)->GetWay())
						{
						case 0:
							(*iter)->SetMovingUp(true);
							break;
						case 1:
							(*iter)->SetMovingRightUp(true);
							break;
						case 2:
							(*iter)->SetMovingRight(true);
							break;
						case 3:
							(*iter)->SetMovingRightDown(true);
							break;
						case 4:
							(*iter)->SetMovingDown(true);
							break;
						case 5:
							(*iter)->SetMovingLeftDown(true);
							break;
						case 6:
							(*iter)->SetMovingLeft(true);
							break;
						case 7:
							(*iter)->SetMovingLeftUp(true);
							break;
						default:
							break;
						}
						(*iter)->OnMove();
						(*iter)->MoveU((*iter)->GetIsMoveNext(), &map);
						(*iter)->MoveRU((*iter)->GetIsMoveNext(), &map);
						(*iter)->MoveR((*iter)->GetIsMoveNext(), &map);
						(*iter)->MoveRD((*iter)->GetIsMoveNext(), &map);
						(*iter)->MoveD((*iter)->GetIsMoveNext(), &map);
						(*iter)->MoveLD((*iter)->GetIsMoveNext(), &map);
						(*iter)->MoveL((*iter)->GetIsMoveNext(), &map);
						(*iter)->MoveLU((*iter)->GetIsMoveNext(), &map);
						(*iter)->SetMovingUp(false);
						(*iter)->SetMovingRight(false);
						(*iter)->SetMovingDown(false);
						(*iter)->SetMovingLeft(false);
						(*iter)->SetMovingRightUp(false);
						(*iter)->SetMovingRightDown(false);
						(*iter)->SetMovingLeftUp(false);
						(*iter)->SetMovingLeftDown(false);
					}
				}
			}
		}
		for (vector<Enemy*>::iterator iter = enemy.begin(); iter != enemy.end(); iter++)
		{
			if ((*iter)->GetLifePoint() <= 0)
			{
				map.SetIndexValue((*iter)->GetIndexX(), (*iter)->GetIndexY(), 0);
				delete (*iter);
				(*iter) = NULL;
				enemy.erase(iter);

				if (enemy.empty())
				{
					break;
				}
				else
				{
					iter = enemy.begin();
				}
			}
			(*iter)->OnMove();
			(*iter)->attackPlayer();
			map.SetIndexValue((*iter)->GetIndexX(), (*iter)->GetIndexY(), 2);
		}
        if (search_count > 5)
        {
			unique_ptr<thread> search(new thread(&CGameStateRun::Search, this));
            if (search->joinable())search->join();
            search_count = 0;
        }
        map.OnMove();
        search_count++;
    }
	if (counter < 4500) in_time = false;
	if (player.empty()) {
		Clear();
		run_init = true;
		fail = true;
		GotoGameState(GAME_STATE_OVER);
	}
	else if (enemy.empty()&& counter>=0)
	{
		Clear();
		run_init = true;
		enemy_all_die = true;
		GotoGameState(GAME_STATE_OVER);
	}
}
void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%
	//
	// 開始載入資料
	//
	furniture.push_back(new Furniture(7, 1, 36));
	furniture.push_back(new Furniture(8, 2, 30));
	furniture.push_back(new Furniture(7, 5, 35));
	furniture.push_back(new Furniture(9, 4, 35));
	furniture.push_back(new Furniture(12, 4, 35));
	furniture.push_back(new Furniture(10, 3, 29));
	furniture.push_back(new Furniture(13, 3, 29));
	furniture.push_back(new Furniture(8, 6, 27));
	furniture.push_back(new Furniture(16, 5, 22));
	furniture.push_back(new Furniture(16, 4, 45));
	furniture.push_back(new Furniture(18, 5, 4));
	furniture.push_back(new Furniture(21, 5, 45));
	furniture.push_back(new Furniture(23, 2, 36));
	furniture.push_back(new Furniture(24, 3, 26));
	furniture.push_back(new Furniture(20, 1, 17));
	furniture.push_back(new Furniture(28, 1, 1));
	furniture.push_back(new Furniture(30, 1, 1));
	furniture.push_back(new Furniture(28, 9, 46));
	furniture.push_back(new Furniture(30, 9, 46));
	furniture.push_back(new Furniture(30, 5, 42));
	furniture.push_back(new Furniture(5, 20, 39));
	furniture.push_back(new Furniture(1, 18, 35));
	furniture.push_back(new Furniture(1, 17, 9));
	furniture.push_back(new Furniture(2, 17, 9));
	furniture.push_back(new Furniture(3, 17, 9));
	furniture.push_back(new Furniture(1, 19, 11));
	furniture.push_back(new Furniture(2, 19, 11));
	furniture.push_back(new Furniture(3, 19, 11));
	furniture.push_back(new Furniture(4, 18, 10));
	furniture.push_back(new Furniture(1, 10, 37));
	furniture.push_back(new Furniture(2, 10, 37));
	furniture.push_back(new Furniture(6, 10, 5));
	furniture.push_back(new Furniture(3, 14, 33));
	furniture.push_back(new Furniture(3, 13, 17));
	furniture.push_back(new Furniture(3, 15, 19));
	furniture.push_back(new Furniture(2, 14, 28));
	furniture.push_back(new Furniture(5, 14, 26));
	furniture.push_back(new Furniture(7, 14, 45));
	furniture.push_back(new Furniture(7, 15, 38));
	furniture.push_back(new Furniture(7, 16, 45));
	furniture.push_back(new Furniture(7, 18, 1));
	furniture.push_back(new Furniture(9, 12, 37));
	furniture.push_back(new Furniture(10, 12, 37));
	furniture.push_back(new Furniture(9, 13, 15));
	furniture.push_back(new Furniture(10, 13, 15));
	furniture.push_back(new Furniture(9, 18, 44));
	furniture.push_back(new Furniture(12, 16, 35));
	furniture.push_back(new Furniture(15, 16, 35));
	furniture.push_back(new Furniture(11, 16, 12));
	furniture.push_back(new Furniture(12, 15, 9));
	furniture.push_back(new Furniture(13, 15, 9));
	furniture.push_back(new Furniture(14, 15, 9));
	furniture.push_back(new Furniture(15, 15, 9));
	furniture.push_back(new Furniture(16, 15, 9));
	furniture.push_back(new Furniture(17, 15, 9));
	furniture.push_back(new Furniture(12, 17, 11));
	furniture.push_back(new Furniture(13, 17, 11));
	furniture.push_back(new Furniture(14, 17, 11));
	furniture.push_back(new Furniture(15, 17, 11));
	furniture.push_back(new Furniture(16, 17, 11));
	furniture.push_back(new Furniture(17, 17, 11));
	furniture.push_back(new Furniture(18, 16, 10));
	furniture.push_back(new Furniture(12, 20, 39));
	furniture.push_back(new Furniture(13, 20, 31));
	furniture.push_back(new Furniture(19, 20, 23));
	furniture.push_back(new Furniture(19, 19, 33));
	furniture.push_back(new Furniture(19, 18, 21));
	furniture.push_back(new Furniture(13, 12, 32));
	furniture.push_back(new Furniture(15, 12, 38));
	furniture.push_back(new Furniture(18, 12, 45));
	furniture.push_back(new Furniture(20, 12, 45));
	furniture.push_back(new Furniture(22, 19, 8));
	furniture.push_back(new Furniture(25, 20, 19));
	furniture.push_back(new Furniture(28, 20, 2));
	furniture.push_back(new Furniture(25, 14, 41));
	furniture.push_back(new Furniture(28, 14, 25));
	furniture.push_back(new Furniture(29, 14, 46));
	furniture.push_back(new Furniture(25, 17, 33));
	furniture.push_back(new Furniture(25, 16, 9));
	furniture.push_back(new Furniture(26, 16, 9));
	furniture.push_back(new Furniture(25, 18, 11));
	furniture.push_back(new Furniture(26, 18, 11));
	furniture.push_back(new Furniture(24, 16, 10));
	furniture.push_back(new Furniture(27, 16, 12));
	enemy.push_back(new Enemy(3, 3, 4));
	enemy.push_back(new Enemy(5, 1, 5));
	enemy.push_back(new Enemy(5, 15, 0));
	enemy.push_back(new Enemy(9, 3, 2));
	enemy.push_back(new Enemy(28, 1, 4));
	enemy.push_back(new Enemy(27, 18, 7));
	enemy.push_back(new Enemy(24, 2, 6));
	enemy.push_back(new Enemy(18, 4, 0));
	enemy.push_back(new Enemy(9, 10, 0));
	player.push_back(new Soldier(1, 1, 100));
	player.push_back(new Soldier(1, 2, 100));
	player.push_back(new Soldier(1, 3, 100));
	player.push_back(new Soldier(1, 4, 100));
	for (vector<Furniture*>::iterator iter = furniture.begin(); iter != furniture.end(); iter++) {
		(*iter)->LoadBitmap();
	}
	for (vector<Enemy*>::iterator iter = enemy.begin(); iter != enemy.end(); iter++)
	{
		(*iter)->LoadBitmap();
	}
	for (vector<Soldier*>::iterator iter = player.begin();iter != player.end();iter++)
	{
		(*iter)->LoadBitmap();
	}
	if(run_init_load)
	{
	CAudio::Instance()->Load(AUDIO_NEWGAME_1, "Sounds\\newgame.mp3");
	CAudio::Instance()->Load(AUDIO_NEWGAME_2, "Sounds\\newgame.mp3");
	CAudio::Instance()->Load(AUDIO_NEWGAME_3, "Sounds\\newgame.mp3");
	CAudio::Instance()->Load(AUDIO_NEWGAME_4, "Sounds\\newgame.mp3");
	CAudio::Instance()->Load(AUDIO_OK, "Sounds\\ok.mp3");
	CAudio::Instance()->Load(AUDIO_FUCKYEA, "Sounds\\USSR.mp3");
	CAudio::Instance()->Load(AUDIO_LMG, "Sounds\\LMG.mp3");
	pause.LoadBitmap();
	background.LoadBitmap(IDB_BACKGROUND);					// 載入背景的圖形
	map.LoadBitmap();
	help.LoadBitmap(IDB_HELP, RGB(255, 255, 255));				// 載入說明的圖形
	corner.LoadBitmap(IDB_CORNER);							// 載入角落圖形
	corner.ShowBitmap(background);							// 將corner貼到background
	run_init_load = false;
	}
    //
    // 完成部分Loading動作，提高進度
    //
    ShowInitProgress(50);
    Sleep(300); // 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
    //
    // 繼續載入其他資料
    //
}
void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_LEFT = 0x41; // keyboard向左看
    const char KEY_UP = 0x57; // keyboard向上看
    const char KEY_RIGHT = 0x44; // keyboard向右看
    const char KEY_DOWN = 0x53; // keyboard向下看
	const char KEY_1 = 0x31;	//武器1(大槍)
	const char KEY_2 = 0x32;	//武器2(手槍)
	const char KEY_3 = 0x33;	//
	const char KEY_4 = 0x34;	//手雷
	const char KEY_ENTER = 13;
    map.OnKeyDown(nChar);
	if (nChar == KEY_ENTER)
	{
		CAudio::Instance()->Play(AUDIO_FUCKYEA, true);
		CAudio::Instance()->Play(AUDIO_LMG,false);
		pause.SetSoviet(true);
		for (vector<Soldier*>::iterator iter = player.begin();iter != player.end();iter++)
		{
			(*iter)->superman();
		}
		for (vector<Enemy*>::iterator iter = enemy.begin(); iter != enemy.end(); iter++)
		{
			(*iter)->SetIsSaw(true);
		}
	}
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_LEFT = 0x41; // keyboard向左看
    const char KEY_UP = 0x57; // keyboard向上看
    const char KEY_RIGHT = 0x44; // keyboard向右看
    const char KEY_DOWN = 0x53; // keyboard向下看
}
void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
    TRACE("Mouse left button down\n");
	for (vector<Soldier*>::iterator iter = player.begin();iter != player.end();iter++)
	{
		if ((*iter)->IsSetRoadLine(point))
		{
			TRACE("Get choosen\n");
			(*iter)->SetChoosen(true);
			break;
		}
		else if ((*iter)->IsSetAction(point))
		{
			TRACE("Action\n");
		}

	}
    if (point.x >= 1200 && point.x <= 1280 && point.y >= 880 && point.y <= 960)
    {
        pause.SetChoosen(true);
    }
}
void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)
{
    TRACE("Mouse left button up\n");
	for (vector<Soldier*>::iterator iter = player.begin();iter != player.end();iter++)
	{
		(*iter)->SetChoosen(false);
		(*iter)->SetInRoadLine(false);
	}
    if (point.x >= 1200 && point.x <= 1280 && point.y >= 880 && point.y <= 960 && pause.isChoosen())
    {
        if (!pause.GetPause())
            pause.SetPause(true);
        else
            pause.SetPause(false);
    }
}
void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
    static int mouse_x, mouse_y;
    mouse_x = point.x / SIZE;
    mouse_y = point.y / SIZE;
	for (vector<Soldier*>::iterator iter = player.begin();iter != player.end();iter++)
	{
		if ((*iter)->IsChoosen())
		{
			(*iter)->SetRoadLine(mouse_x, mouse_y, &map);
		}
	}
}
void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}
void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}
void CGameStateRun::GetMouse(UINT nFlags, CPoint point)
{
}
void CGameStateRun::OnShow()
{
	CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
	CFont f, *fp;
	f.CreatePointFont(200, "Times New Roman");	// 產生 font f; 200表示20 point的字
	fp = pDC->SelectObject(&f);					// 選用 font f
	pDC->SetBkColor(RGB(0, 0, 0));
	pDC->SetTextColor(RGB(255, 255, 255));
	char str[80];								// Demo 數字對字串的轉換
	sprintf(str, "時間剩下:%d", counter / 30);
	pDC->TextOut(900, 900, str);
	pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
	CDDraw::ReleaseBackCDC();
    map.OnShow();
	for (vector<Soldier*>::iterator iter = player.begin();iter != player.end();iter++)
	{
		(*iter)->OnShow();
	}
    corner.SetTopLeft(0, 0);
    corner.ShowBitmap();
    corner.SetTopLeft(SIZE_X - corner.Width(), SIZE_Y - corner.Height());
    corner.ShowBitmap();
    pause.OnShow();
	for (vector<Furniture*>::iterator iter = furniture.begin(); iter != furniture.end(); iter++) {
		(*iter)->OnShow();
	}
    for (vector<Enemy*>::iterator iter = enemy.begin(); iter != enemy.end(); iter++)
    {
        (*iter)->OnShow();
    }
}
}