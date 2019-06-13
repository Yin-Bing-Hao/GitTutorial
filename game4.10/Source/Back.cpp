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
	Back::Back(int NUM, int Lux, int Luy) {
		Initialize();
		floor = NUM;
		X = Lux;
		Y = Luy;
	}
	Back::~Back(){}
	void Back::Initialize()
	{
		isLight = false;
	}
	void Back::LoadBitmap()
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
	void Back::OnShow()
	{
		switch (floor)
			{
			case 1:
				if (isLight) {
					wood_light.SetTopLeft(X , Y );
					wood_light.ShowBitmap();
				}
				else {
					wood.SetTopLeft(X , Y );
					wood.ShowBitmap();
				}
				break;
			case 2:
				if (isLight) {
					sand_light.SetTopLeft(X , Y );
					sand_light.ShowBitmap();
				}
				else {
					sand.SetTopLeft(X , Y );
					sand.ShowBitmap();
				}
				break;
			case 3:
				sidewalk.SetTopLeft(X + MH, Y + MW);
				sidewalk.ShowBitmap();
				break;
			case 4:
				if (isLight) {
					floor_1_Light.SetTopLeft(X, Y);
					floor_1_Light.ShowBitmap();
				}
				else {
					floor_1.SetTopLeft(X, Y);
					floor_1.ShowBitmap();
				}
				break;
			case 5:
				if (isLight) {
					floor_2_Light.SetTopLeft(X, Y);
					floor_2_Light.ShowBitmap();
				}
				else {
					floor_2.SetTopLeft(X, Y);
					floor_2.ShowBitmap();
				}
				break;
			case 6:
				if (isLight) {
					floor_3_Light.SetTopLeft(X , Y );
					floor_3_Light.ShowBitmap();
				}
				else {
					floor_3.SetTopLeft(X , Y );
					floor_3.ShowBitmap();
				}
				break;
			default:
				break;
			}
	}
	void Back::SetLight(bool flag)
	{
		isLight = flag;
	}
	void Back::SetObject(int num)
	{
		ObjectNum = num;
	}
}