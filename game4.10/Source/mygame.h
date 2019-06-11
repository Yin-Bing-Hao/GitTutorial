/*
 * mygame.h: ���ɮ��x�C��������class��interface
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
//#include "CBall.h"
#include "Furniture.h"
#include "CBouncingBall.h"
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

enum AUDIO_ID  				// �w�q�U�ح��Ī��s��
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
	AUDIO_P9_3
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C���}�Y�e������
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////
class Back;

class CGameStateInit : public CGameState
{
    public:
        CGameStateInit(CGame* g);
        void OnInit();  								// �C������Ȥιϧγ]�w
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnKeyUp(UINT, UINT, UINT); 				// �B�z��LUp���ʧ@
        void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
    protected:
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
		void OnMove();
    private:
        CMovingBitmap logo;								// csie��logo
		CMovingBitmap background;
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameMap
{
    public:
        CGameMap();
        void LoadBitmap();
        void OnShow();
        int GetIndexValue(int, int);
        void SetIndexValue(int, int, int);
        void OnMove();
        void OnKeyDown(UINT);
		Back GetBackGround();
		void SetBackLight(int,int, bool flag);
        void RandomBouncingBall();
        void InitializeBouncingBall(int, int, int);
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
        int random_num;
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
        ~CGamePauseButton();
    protected:
        const int X, Y;
        CMovingBitmap stop, start;
		bool isPause;
        bool isChoose;
};



class CGameStateRun : public CGameState
{
    public:
        CGameStateRun(CGame* g);
        ~CGameStateRun();
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnInit();  								// �C������Ȥιϧγ]�w
        void OnKeyDown(UINT, UINT, UINT);
        void OnKeyUp(UINT, UINT, UINT);
        void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
        void OnLButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
        // �B�z�ƹ����ʧ@
        void OnMouseMove(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
        void OnRButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
        void OnRButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
        void GetMouse(UINT nFlags, CPoint point);
		void Search();

    protected:
        void OnMove();									// ���ʹC������
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
		void Clear();
    private:
        CGameMap		map;
        CGamePauseButton pause;
        CMovingBitmap	background;	// �I����
        CMovingBitmap	help;		// ������
        CMovingBitmap	corner;		// ������
        vector<Soldier*>player;		// ��l
		vector<Enemy*>	enemy;		// �ĤH��
		vector<Furniture*> furniture;

};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����������A(Game Over)
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateOver : public CGameState
{
    public:
        CGameStateOver(CGame* g);
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnInit();
    protected:
        void OnMove();									// ���ʹC������
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
    private:
        int counter;	// �˼Ƥ��p�ƾ�
};

}

#endif // !MYGAME_H