#include "stdafx.h"
#include "Resource.h"
#include "MainFrm.h"
#include "gameDoc.h"
#include <mmsystem.h>
#include <windows.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Back.h"
#include "mygame.h"
namespace game_framework {
	Back::Back(){}
	Back::Back(int NUM, int Lux, int Luy) {	//背景(圖片編號，X軸，Y軸)
		Initialize();
		floor = NUM;
		X = Lux;
		Y = Luy;
	}
	Back::~Back(){}
	void Back::Initialize()	//設定初始值
	{
		isLight = false;	//初始值：圖片未看到(灰暗)
	}
	void Back::LoadBitmap()	//圖片讀取
	{
			sand.LoadBitmap("Bitmaps/sand.bmp", RGB(255, 255, 255));
			wood.LoadBitmap("Bitmaps/wood.bmp", RGB(255, 255, 255));
			sidewalk.LoadBitmap(BACK_SIDEWALK, RGB(255, 255, 255));
			sand_light.LoadBitmap("Bitmaps/sand_light.bmp", RGB(255, 255, 255));
			wood_light.LoadBitmap("Bitmaps/wood_light.bmp", RGB(255, 255, 255));
			floor_1.LoadBitmap("Bitmaps/floor_1.bmp", RGB(255, 255, 255));
			floor_1_Light.LoadBitmap("Bitmaps/floor_1_Light.bmp", RGB(255, 255, 255));
			floor_2.LoadBitmap("Bitmaps/floor_2.bmp", RGB(255, 255, 255));
			floor_2_Light.LoadBitmap("Bitmaps/floor_2_Light.bmp", RGB(255, 255, 255));
			floor_3.LoadBitmap("Bitmaps/floor_3.bmp", RGB(255, 255, 255));
			floor_3_Light.LoadBitmap("Bitmaps/floor_3_Light.bmp", RGB(255, 255, 255));
	}
	void Back::OnShow()	//顯示
	{
		switch (floor)	//地板種類
			{
			case 1:
				if (isLight) {
					wood_light.SetTopLeft(X , Y );
					wood_light.ShowBitmap();			//顯示
				}
				else {
					wood.SetTopLeft(X , Y );
					wood.ShowBitmap();					//顯示
				}
				break;
			case 2:
				if (isLight) {
					sand_light.SetTopLeft(X , Y );
					sand_light.ShowBitmap();			//顯示
				}
				else {
					sand.SetTopLeft(X , Y );
					sand.ShowBitmap();					//顯示
				}
				break;
			case 3:
				sidewalk.SetTopLeft(X + MH, Y + MW);	//人物路線的紅線
				sidewalk.ShowBitmap();					//顯示
				break;
			case 4:
				if (isLight) {
					floor_1_Light.SetTopLeft(X, Y);
					floor_1_Light.ShowBitmap();			//顯示
				}
				else {
					floor_1.SetTopLeft(X, Y);
					floor_1.ShowBitmap();				//顯示
				}
				break;
			case 5:
				if (isLight) {
					floor_2_Light.SetTopLeft(X, Y);
					floor_2_Light.ShowBitmap();			//顯示
				}
				else {
					floor_2.SetTopLeft(X, Y);
					floor_2.ShowBitmap();				//顯示
				}
				break;
			case 6:
				if (isLight) {
					floor_3_Light.SetTopLeft(X , Y );
					floor_3_Light.ShowBitmap();			//顯示
				}
				else {
					floor_3.SetTopLeft(X , Y );
					floor_3.ShowBitmap();				//顯示
				}
				break;
			default:
				break;
			}
	}
	void Back::SetLight(bool flag)	//設定光亮
	{
		isLight = flag;				//設定以有無被看到，來此背景是否發亮
	}
	void Back::SetObject(int num)	//設定圖片編號
	{
		ObjectNum = num;
	}
}