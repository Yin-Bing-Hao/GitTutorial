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
	Back::Back()
	{

	}
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