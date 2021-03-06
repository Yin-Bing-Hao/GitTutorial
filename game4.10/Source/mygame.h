/*
 * mygame.h: 本檔案儲遊戲本身的class的interface
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
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and Soldier.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
*/

#include "Soldier.h"
#include "Enemy.h"
#include "Furniture.h"
#include "Back.h"


#ifndef MYGAME_H
#define MYGAME_H



#define ROW 32
#define COL 24
#define SIZE 40
namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////

enum AUDIO_ID  				// 定義各種音效的編號
{
	AUDIO_INIT_BACKGRUOND,
    AUDIO_DING,				// 0
    AUDIO_LAKE,				// 1
    AUDIO_NTUT,				// 2
	AUDIO_HK416_1, 
	AUDIO_HK416_2, 
	AUDIO_HK416_3, 
	AUDIO_HK416_4,
	AUDIO_P9_1,
	AUDIO_P9_2,
	AUDIO_P9_3,
	AUDIO_MISSION_FAIL,
	AUDIO_MISSION_COMPLETE,
	AUDIO_NEWGAME_1,
	AUDIO_NEWGAME_2,
	AUDIO_NEWGAME_3,
	AUDIO_NEWGAME_4,
	AUDIO_OK,
	AUDIO_FUCKYEA,
	AUDIO_LMG
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////
class Back;

class CGameStateInit : public CGameState
{
    public:
        CGameStateInit(CGame* g);
        void OnInit();  								// 遊戲的初值及圖形設定
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
        void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
    protected:
        void OnShow();									// 顯示這個狀態的遊戲畫面
		void OnMove();
    private:
        CMovingBitmap logo;								// csie的logo
		CMovingBitmap background;
		CMovingBitmap about;
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameMap
{
    public:
        CGameMap();
        void LoadBitmap();
        void OnShow();
		void OnMove();
        int GetIndexValue(int, int);
        void SetIndexValue(int, int, int);
        void OnKeyDown(UINT);
		Back GetBackGround();
		void SetBackLight(int,int, bool flag);
        ~CGameMap();
    protected:
        CMovingBitmap RightUpCorner, LeftUpCorner, RightDownCorner, LeftDownCorner, Horizonal, Vertical;
        CMovingBitmap wall_Horizontal, wall_Straight, wall_LU, wall_LD, wall_RU, wall_RD, wall_Invertr_T, wall_T,wall_RT,wall_LT;
        CMovingBitmap glass_Straight, glass_Horizontal, glass_BrokenStraight, glass_BrokenHorizontal;
        CMovingBitmap door_StrUp, door_StrDown, door_HorRight, door_HorLeft;
		CMovingBitmap sand, wood, sidewalk, sand_light, wood_light;
		int map[COL][ROW];
        const int X, Y;
        const int MW, MH;
        //CBouncingBall* bballs;
		Back** Background;
};
class CGamePauseButton
{
    public:
		CGamePauseButton();
        void LoadBitmap();
        void OnShow();
		void SetPause(bool flag);
		bool GetPause();
        void SetChoosen(bool flag);
        bool isChoosen();
		void SetSoviet(bool);
		bool GetSoviet();
        ~CGamePauseButton();
    protected:
        const int X, Y;
        CMovingBitmap stop, start,soviet;
		bool isPause;
        bool isChoose;
		bool isSoviet;
};
class CGameStateRun : public CGameState
{
    public:
        CGameStateRun(CGame* g);
        ~CGameStateRun();
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnInit();  								// 遊戲的初值及圖形設定
        void OnKeyDown(UINT, UINT, UINT);
        void OnKeyUp(UINT, UINT, UINT);
        void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
        void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
        // 處理滑鼠的動作
        void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作
        void OnRButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
        void OnRButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
        void GetMouse(UINT nFlags, CPoint point);
		void Search();
    protected:
        void OnMove();									// 移動遊戲元素
        void OnShow();									// 顯示這個狀態的遊戲畫面
		void Clear();
    private:
        CGameMap		map;
        CGamePauseButton pause;
        CMovingBitmap	background;	// 背景圖
        CMovingBitmap	help;		// 說明圖
        CMovingBitmap	corner;		// 角落圖
        vector<Soldier*>player;		// 玩家們
		vector<Enemy*>	enemy;		// 敵人們
		vector<Furniture*> furniture;//家具
		int counter;	//計數器

};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateOver : public CGameState
{
    public:
        CGameStateOver(CGame* g);
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnInit();
		void OnLButtonDown(UINT nFlags, CPoint point);
    protected:
        void OnMove();									// 移動遊戲元素
        void OnShow();									// 顯示這個狀態的遊戲畫面
    private:
        int counter;	// 倒數之計數器
		CMovingBitmap One_Star, Two_Star, Three_Star, Fail;
};
}

#endif // !MYGAME_H