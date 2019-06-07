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
    logo.LoadBitmap(IDB_BACKGROUND);
    Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
    //
    // 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
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
        GotoGameState(GAME_STATE_RUN);						// 切換至GAME_STATE_RUN
    else if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
        PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
    GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
}

void CGameStateInit::OnShow()
{
    //
    // 貼上logo
    //
    logo.SetTopLeft((SIZE_X - logo.Width()) / 2, SIZE_Y / 8);
    logo.ShowBitmap();
    //
    // Demo螢幕字型的使用，不過開發時請盡量避免直接使用字型，改用CMovingBitmap比較好
    //
    CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
    CFont f, *fp;
    f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
    fp = pDC->SelectObject(&f);					// 選用 font f
    pDC->SetBkColor(RGB(0, 0, 0));
    pDC->SetTextColor(RGB(255, 255, 0));
    pDC->TextOut(120, 220, "Please click mouse or press SPACE to begin.");
    pDC->TextOut(5, 395, "Press Ctrl-F to switch in between window mode and full screen mode.");

    if (ENABLE_GAME_PAUSE)
        pDC->TextOut(5, 425, "Press Ctrl-Q to pause the Game.");

    pDC->TextOut(5, 455, "Press Alt-F4 or ESC to Quit.");
    pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
    CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	//delete pDC;
}

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
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
    // 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
    //     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
    //
    ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
    //
    // 開始載入資料
    //
    Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
    //
    // 最終進度為100%
    //
    ShowInitProgress(100);
}

void CGameStateOver::OnShow()
{
    CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
    CFont f, *fp;
    f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
    fp = pDC->SelectObject(&f);					// 選用 font f
    pDC->SetBkColor(RGB(0, 0, 0));
    pDC->SetTextColor(RGB(255, 255, 0));
    char str[80];								// Demo 數字對字串的轉換
    sprintf(str, "Game Over ! (%d)", counter / 30);
    pDC->TextOut(240, 210, str);
    pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
    CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
}

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////
#pragma region CGameMap
CGameMap::CGameMap() : X(0), Y(0), MW(SIZE), MH(SIZE)
{
    random_num = 0;
    int map_init[COL][ROW] =
    {
        {5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 6},
        {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
        {3, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
        {3, 0, 3, 0, 5, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 10, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 6, 0, 0, 0, 3},
        {3, 0, 3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3},
        {3, 0, 3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3},
        {3, 0, 3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3},
        {3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
        {3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3},
        {3, 0, 7, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3},
        {3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3},
        {3, 0, 3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3},
        {3, 0, 3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3},
        {3, 0, 3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3},
        {3, 0, 3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3},
        {3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
        {3, 0, 3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3},
        {3, 0, 3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3},
        {3, 0, 3, 0, 7, 4, 4, 4, 4, 4, 4, 11, 4, 4, 4, 4, 9, 4, 4, 4, 13, 4, 4, 0, 4, 4, 4, 8, 0, 0, 0, 3},
        {3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
        {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
        {7, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 8},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2}

    };
    const int back_init[48][64] =
    {
        {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},//0
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3},//1
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3},//2
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3},//3
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3},//4
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3},//5
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3},//6
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//7
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//8
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//9
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//10
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//11
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//12
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//13
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//14
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//15
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//16
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//17
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//18
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//19
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//20
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//21
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3}, //22
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//23
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//24
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//25
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//26
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//27
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//28
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//29
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3}, //30
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//31
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//32
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//33
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//34
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//35
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3},//36
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3},//37
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3},//38
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3},//39
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3},//40
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3},//41
        {3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3},//42
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
            //Background[i * 32 + j] = new Back(back_init[i][j], back_init[i + 1][j], back_init[i][j + 1], back_init[i + 1][j + 1], j * 40, i * 40);
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

    map[1][1] = 1;  //出生點
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

    for (int i = 0; i < 48; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            Background[i][j].LoadBitmap();
        }
    }
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
                    glass_Horizontal.SetTopLeft(X + (MW * i), Y + (MH * j));
                    glass_Horizontal.ShowBitmap();
                    break;

                case 12:
                    glass_Straight.SetTopLeft(X + (MW * i), Y + (MH * j));
                    glass_Straight.ShowBitmap();
                    break;

                case 13:
                    glass_BrokenHorizontal.SetTopLeft(X + (MW * i), Y + (MH * j));
                    glass_BrokenHorizontal.ShowBitmap();
                    break;

                case 14:
                    glass_BrokenStraight.SetTopLeft(X + (MW * i), Y + (MH * j));
                    glass_BrokenStraight.ShowBitmap();
                    break;

                case 15:
                    door_HorLeft.SetTopLeft(X + (MW * i), Y + (MH * j));
                    door_HorLeft.ShowBitmap();
                    break;

                case 16:
                    door_HorRight.SetTopLeft(X + (MW * i), Y + (MH * j));
                    door_HorRight.ShowBitmap();
                    break;

                case 17:
                    door_StrUp.SetTopLeft(X + (MW * i), Y + (MH * j));
                    door_StrUp.ShowBitmap();
                    break;

                case 18:
                    door_StrDown.SetTopLeft(X + (MW * i), Y + (MH * j));
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
    return map[y][x];
}
void CGameMap::SetIndexValue(int x, int y, int value)
{
    map[y][x] = value;
}

void CGameMap::InitializeBouncingBall(int ini_index, int row, int col)
{
    /*const int VELOCITY = 10;
    const int BALL_PLC_HEIGHT = 15;
    int floor = Y + (row + 1) * MH - BALL_PLC_HEIGHT;
    bballs[ini_index].LoadBitmap();
    bballs[ini_index].SetFloor(floor);
    bballs[ini_index].SetVelocity(VELOCITY + col);
    bballs[ini_index].SetXY(X + col * MW + MW / 2, floor);*/
}

void CGameMap::RandomBouncingBall()
{
    /*const int MAX_RAND_NUM = 10;
    random_num = (rand() % MAX_RAND_NUM) + 1;
    //bballs = new CBouncingBall[random_num];
    int ini_index = 0;

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 5; col++)
        {
            if (map[row][col] != 0 && ini_index < random_num)
            {
                InitializeBouncingBall(ini_index, row, col);
                ini_index++;
            }
        }
    }*/
}

void CGameMap::OnKeyDown(UINT nChar)
{
    const int KEY_SPACE = 0x20;
    /*if (nChar == KEY_SPACE)
    	RandomBouncingBall();*/
}

Back CGameMap::GetBackGround()
{
    return **Background;
}

void CGameMap::SetBackLight(int y, int x, bool flag)
{
    Background[y][x].SetLight(flag);
}

void CGameMap::OnMove()
{
    /*for (int i = 0; i < random_num; i++)
    {
        bballs[i].OnMove();
    }*/
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



CGamePauseButton::CGamePauseButton() : X(1200), Y(880) {}
void CGamePauseButton::LoadBitmap()
{
    stop.LoadBitmap(GAME_STOP, RGB(255, 255, 255));
    start.LoadBitmap(GAME_START, RGB(255, 255, 255));
}

void CGamePauseButton::OnShow()
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

CGameStateRun::~CGameStateRun()
{
	for (vector<Enemy*>::iterator iter = enemy.begin();iter != enemy.end();iter++)
	{
		delete (*iter);
	}
	enemy.clear();
	TRACE("Enemy vector size:%d\n", sizeof(enemy));
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
    //for (int i = 0; i < NUMBALLS; i++)  				// 設定球的起始座標
    //{
    //	int x_pos = i % BALL_PER_ROW;
    //	int y_pos = i / BALL_PER_ROW;
    //	ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
    //	ball[i].SetDelay(x_pos);
    //	ball[i].SetIsAlive(true);
    //}
    background.SetTopLeft(BACKGROUND_X, 0);				// 設定背景的起始座標
    help.SetTopLeft(0, SIZE_Y - help.Height());			// 設定說明圖的起始座標
    //CAudio::Instance()->Play(AUDIO_NTUT, true);			// 撥放 MIDI
}
void CGameStateRun::Search()
{
    Search_mutex.lock();
	//unique_ptr<thread> search_enemy(new thread(&Soldier::searchEnemy, &people, &map, enemy));
	
    for (int i = 0; i < 48; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            map.SetBackLight(i, j, false);
        }
    }

    for (vector<Enemy*>::iterator iter = enemy.begin(); iter != enemy.end(); iter++)
    {
        (*iter)->SetIsSaw(false);
    }
	people.searchEnemy(&map, enemy);
    //if (search_enemy->joinable()) search_enemy->join();
    Search_mutex.unlock();
}
void CGameStateRun::OnMove()							// 移動遊戲元素
{
    

    //
    // 如果希望修改cursor的樣式，則將下面程式的commment取消即可
    //
    // SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
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
        //people.Perspective(map);
        vector<Line*> ptr = people.GetRoadLine();
        people.attackEnemy();

        if (!ptr.empty())
        {
            switch (people.GetWay())
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
            map.SetIndexValue(people.GetIndexX(), people.GetIndexY(), 1);
            people.MoveU(people.GetIsMoveNext(), &map);
            people.MoveRU(people.GetIsMoveNext(), &map);
            people.MoveR(people.GetIsMoveNext(), &map);
            people.MoveRD(people.GetIsMoveNext(), &map);
            people.MoveD(people.GetIsMoveNext(), &map);
            people.MoveLD(people.GetIsMoveNext(), &map);
            people.MoveL(people.GetIsMoveNext(), &map);
            people.MoveLU(people.GetIsMoveNext(), &map);
            people.SetMovingUp(false);
            people.SetMovingRight(false);
            people.SetMovingDown(false);
            people.SetMovingLeft(false);
            people.SetMovingRightUp(false);
            people.SetMovingRightDown(false);
            people.SetMovingLeftUp(false);
            people.SetMovingLeftDown(false);
        }

        if (search_count > 5)
        {
			/*unique_ptr<thread> search(new thread(&CGameStateRun::Search, this));

            if (search->joinable())search->join();*/
			Search();

            search_count = 0;
        }

        for (vector<Enemy*>::iterator iter = enemy.begin(); iter != enemy.end(); iter++)
        {
			if ((*iter)->GetLifePoint() <= 0)
			{
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
            map.SetIndexValue((*iter)->GetIndexX(), (*iter)->GetIndexY(), 2);

            //TRACE("Enemy_index:%d %d\n", (*iter)->GetIndexX(), (*iter)->GetIndexY());
            //TRACE("ENEMYLIFE %d\n", (*iter)->GetLifePoint());
            
        }

        /*for (int i = 0;i < COL;i++)
        {
        	for (int j = 0;j < ROW;j++)
        	{
        		if(map.GetIndexValue(j,i)==1)
        			TRACE("Soldier in map %d %d\n", j, i);
        	}
        }*/
        //
        // 移動擦子
        //
        map.OnMove();
        search_count++;
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
    enemy.push_back(new Enemy(1, 5, 0));
    enemy.push_back(new Enemy(5, 1, 0));
	enemy.push_back(new Enemy(8, 2, 0));
    for (vector<Enemy*>::iterator iter = enemy.begin(); iter != enemy.end(); iter++)
    {
        (*iter)->LoadBitmap();
    }
	CAudio::Instance()->Load(AUDIO_HK416_1, "Sounds\\AR15_gun_sound.mp3");
	CAudio::Instance()->Load(AUDIO_HK416_2, "Sounds\\AR15_gun_sound.mp3");
	CAudio::Instance()->Load(AUDIO_HK416_3, "Sounds\\AR15_gun_sound.mp3");
	CAudio::Instance()->Load(AUDIO_HK416_4, "Sounds\\AR15_gun_sound.mp3");
	CAudio::Instance()->Load(AUDIO_P9_1, "Sounds\\P9.mp3");
	CAudio::Instance()->Load(AUDIO_P9_2, "Sounds\\P9.mp3");
	CAudio::Instance()->Load(AUDIO_P9_3, "Sounds\\P9.mp3");
	people.LoadBitmap();
    pause.LoadBitmap();
    background.LoadBitmap(IDB_BACKGROUND);					// 載入背景的圖形
    map.LoadBitmap();
    //
    // 完成部分Loading動作，提高進度
    //
    ShowInitProgress(50);
    Sleep(300); // 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
    //
    // 繼續載入其他資料
    //
    help.LoadBitmap(IDB_HELP, RGB(255, 255, 255));				// 載入說明的圖形
    corner.LoadBitmap(IDB_CORNER);							// 載入角落圖形
    corner.ShowBitmap(background);							// 將corner貼到background
    //CAudio::Instance()->Load(AUDIO_NTUT,  "sounds\\song.mid");	// 載入編號2的聲音ntut.mid
    //
    // 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
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

	if (people.IsInRoadLine()) {
		if (nChar == KEY_1) {
			people.ChangeGun(0);
		}
		else if (nChar == KEY_2) {
			people.ChangeGun(1);
		}
		else if (nChar == KEY_3) {
			people.ChangeGun(2);
		}
		else if (nChar == KEY_4) {
			people.ChangeGun(3);
		}
	}
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_LEFT = 0x41; // keyboard向左看
    const char KEY_UP = 0x57; // keyboard向上看
    const char KEY_RIGHT = 0x44; // keyboard向右看
    const char KEY_DOWN = 0x53; // keyboard向下看

    if (nChar == KEY_LEFT)
        people.SetWatchLeft(false);

    if (nChar == KEY_RIGHT)
        people.SetWatchRight(false);

    if (nChar == KEY_UP)
        people.SetWatchUp(false);

    if (nChar == KEY_DOWN)
        people.SetWatchDown(false);
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
    TRACE("Mouse left button down\n");
    people.TestInRedLine(point);

    if (people.IsSetRoadLine(point))
    {
        TRACE("Get choosen\n");
        people.SetChoosen(true);
    }
    else if (people.IsSetAction(point))
    {
        TRACE("Action\n");
    }

    if (point.x >= 1200 && point.x <= 1280 && point.y >= 880 && point.y <= 960)
    {
        pause.SetChoosen(true);
    }
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)
{
    TRACE("Mouse left button up\n");
    people.SetChoosen(false);
	people.SetInRoadLine(false);
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

    //TRACE("%d,%d\n", point.x, point.y);
    if (people.IsChoosen())
    {
        people.SetRoadLine(mouse_x, mouse_y, &map);
    }
	if (people.IsInRoadLine())
	{

	}
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
    people.SetMovingRight(true);
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
    people.SetMovingRight(false);
}

void CGameStateRun::GetMouse(UINT nFlags, CPoint point)
{
}

void CGameStateRun::OnShow()
{
    //
    //  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
    //        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
    //        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
    //
    //
    //  貼上背景圖、撞擊數、球、擦子、彈跳的球
    //
    //help.ShowBitmap();					// 貼上說明圖
    map.OnShow();
    SetCursor(LoadCursor(NULL, IDC_CROSS));
    people.OnShow();
    corner.SetTopLeft(0, 0);
    corner.ShowBitmap();
    corner.SetTopLeft(SIZE_X - corner.Width(), SIZE_Y - corner.Height());
    corner.ShowBitmap();
    pause.OnShow();

    for (vector<Enemy*>::iterator iter = enemy.begin(); iter != enemy.end(); iter++)
    {
        (*iter)->OnShow();
    }
}
}