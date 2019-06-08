#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include <vector>
#include "mygame.h"
#include "Furniture.h"
#include<iostream>

namespace game_framework {
	Furniture::Furniture(int ix, int iy, int itype)
	{
		x = ix;
		y = iy;
		type = itype;
	}
	Furniture::~Furniture()
	{
	}
	int Furniture::GetX()
	{
		return x;
	}
	int Furniture::GetY()
	{
		return y;
	}

	void Furniture::OnShow()
	{
		if (isSaw) {
			switch (type)
			{
			case 1:
				bed_down.SetTopLeft(x * 40, y * 40);
				bed_down.ShowBitmap();
				break;
			case 2:
				bed_left.SetTopLeft(x * 40, y * 40);
				bed_left.ShowBitmap();
				break;
			case 3:
				bed_up.SetTopLeft(x * 40, y * 40);
				bed_up.ShowBitmap();
				break;
			case 4:
				bed_right.SetTopLeft(x * 40, y * 40);
				bed_right.ShowBitmap();
				break;
			case 5:
				double_bed_down.SetTopLeft(x * 40, y * 40);
				double_bed_down.ShowBitmap();
				break;
			case 6:
				double_bed_left.SetTopLeft(x * 40, y * 40);
				double_bed_left.ShowBitmap();
				break;
			case 7:
				double_bed_up.SetTopLeft(x * 40, y * 40);
				double_bed_up.ShowBitmap();
				break;
			case 8:
				double_bed_right.SetTopLeft(x * 40, y * 40);
				double_bed_right.ShowBitmap();
				break;
			case 9:
				chair_down.SetTopLeft(x * 40, y * 40);
				chair_down.ShowBitmap();
				break;
			case 10:
				chair_left.SetTopLeft(x * 40, y * 40);
				chair_left.ShowBitmap();
				break;
			case 11:
				chair_up.SetTopLeft(x * 40, y * 40);
				chair_up.ShowBitmap();
				break;
			case 12:
				chair_right.SetTopLeft(x * 40, y * 40);
				chair_right.ShowBitmap();
				break;
			case 13:
				office_chair_down.SetTopLeft(x * 40, y * 40);
				office_chair_down.ShowBitmap();
				break;
			case 14:
				office_chair_left.SetTopLeft(x * 40, y * 40);
				office_chair_left.ShowBitmap();
				break;
			case 15:
				office_chair_up.SetTopLeft(x * 40, y * 40);
				office_chair_up.ShowBitmap();
				break;
			case 16:
				office_chair_right.SetTopLeft(x * 40, y * 40);
				office_chair_right.ShowBitmap();
				break;
			case 17:
				sofa_down_1.SetTopLeft(x * 40, y * 40);
				sofa_down_1.ShowBitmap();
				break;
			case 18:
				sofa_left_1.SetTopLeft(x * 40, y * 40);
				sofa_left_1.ShowBitmap();
				break;
			case 19:
				sofa_up_1.SetTopLeft(x * 40, y * 40);
				sofa_up_1.ShowBitmap();
				break;
			case 20:
				sofa_right_1.SetTopLeft(x * 40, y * 40);
				sofa_right_1.ShowBitmap();
				break;
			case 21:
				sofa_down_2.SetTopLeft(x * 40, y * 40);
				sofa_down_2.ShowBitmap();
				break;
			case 22:
				sofa_left_2.SetTopLeft(x * 40, y * 40);
				sofa_left_2.ShowBitmap();
				break;
			case 23:
				sofa_up_2.SetTopLeft(x * 40, y * 40);
				sofa_up_2.ShowBitmap();
				break;
			case 24:
				sofa_right_2.SetTopLeft(x * 40, y * 40);
				sofa_right_2.ShowBitmap();
				break;
			case 25:
				solosofa_down_1.SetTopLeft(x * 40, y * 40);
				solosofa_down_1.ShowBitmap();
				break;
			case 26:
				solosofa_left_1.SetTopLeft(x * 40, y * 40);
				solosofa_left_1.ShowBitmap();
				break;
			case 27:
				solosofa_up_1.SetTopLeft(x * 40, y * 40);
				solosofa_up_1.ShowBitmap();
				break;
			case 28:
				solosofa_right_1.SetTopLeft(x * 40, y * 40);
				solosofa_right_1.ShowBitmap();
				break;
			case 29:
				solosofa_down_2.SetTopLeft(x * 40, y * 40);
				solosofa_down_2.ShowBitmap();
				break;
			case 30:
				solosofa_left_2.SetTopLeft(x * 40, y * 40);
				solosofa_left_2.ShowBitmap();
				break;
			case 31:
				solosofa_up_2.SetTopLeft(x * 40, y * 40);
				solosofa_up_2.ShowBitmap();
				break;
			case 32:
				solosofa_right_2.SetTopLeft(x * 40, y * 40);
				solosofa_right_2.ShowBitmap();
				break;
			case 33:
				circle_H.SetTopLeft(x * 40, y * 40);
				circle_H.ShowBitmap();
				break;
			case 34:
				circle_L.SetTopLeft(x * 40, y * 40);
				circle_L.ShowBitmap();
				break;
			case 35:
				rectangle_H.SetTopLeft(x * 40, y * 40);
				rectangle_H.ShowBitmap();
				break;
			case 36:
				rectangle_L.SetTopLeft(x * 40, y * 40);
				rectangle_L.ShowBitmap();
				break;
			case 37:
				square_down.SetTopLeft(x * 40, y * 40);
				square_down.ShowBitmap();
				break;
			case 38:
				square_left.SetTopLeft(x * 40, y * 40);
				square_left.ShowBitmap();
				break;
			case 39:
				square_up.SetTopLeft(x * 40, y * 40);
				square_up.ShowBitmap();
				break;
			case 40:
				square_right.SetTopLeft(x * 40, y * 40);
				square_right.ShowBitmap();
				break;
			case 41:
				TVtable_down.SetTopLeft(x * 40, y * 40);
				TVtable_down.ShowBitmap();
				break;
			case 42:
				TVtable_left.SetTopLeft(x * 40, y * 40);
				TVtable_left.ShowBitmap();
				break;
			case 43:
				TVtable_up.SetTopLeft(x * 40, y * 40);
				TVtable_up.ShowBitmap();
				break;
			case 44:
				TVtable_right.SetTopLeft(x * 40, y * 40);
				TVtable_right.ShowBitmap();
				break;
			case 45:
				weed_1.SetTopLeft(x * 40, y * 40);
				weed_1.ShowBitmap();
				break;
			case 46:
				weed_2.SetTopLeft(x * 40, y * 40);
				weed_2.ShowBitmap();
				break;

			default:
				break;
			}
		}
		else {
			switch (type)
			{
			case 1:
				bed_down_black.SetTopLeft(x * 40, y * 40);
				bed_down_black.ShowBitmap();
				break;
			case 2:
				bed_left_black.SetTopLeft(x * 40, y * 40);
				bed_left_black.ShowBitmap();
				break;
			case 3:
				bed_up_black.SetTopLeft(x * 40, y * 40);
				bed_up_black.ShowBitmap();
				break;
			case 4:
				bed_right_black.SetTopLeft(x * 40, y * 40);
				bed_right_black.ShowBitmap();
				break;
			case 5:
				double_bed_down_black.SetTopLeft(x * 40, y * 40);
				double_bed_down_black.ShowBitmap();
				break;
			case 6:
				double_bed_left_black.SetTopLeft(x * 40, y * 40);
				double_bed_left_black.ShowBitmap();
				break;
			case 7:
				double_bed_up_black.SetTopLeft(x * 40, y * 40);
				double_bed_up_black.ShowBitmap();
				break;
			case 8:
				double_bed_right_black.SetTopLeft(x * 40, y * 40);
				double_bed_right_black.ShowBitmap();
				break;
			case 9:
				chair_down_black.SetTopLeft(x * 40, y * 40);
				chair_down_black.ShowBitmap();
				break;
			case 10:
				chair_left_black.SetTopLeft(x * 40, y * 40);
				chair_left_black.ShowBitmap();
				break;
			case 11:
				chair_up_black.SetTopLeft(x * 40, y * 40);
				chair_up_black.ShowBitmap();
				break;
			case 12:
				chair_right_black.SetTopLeft(x * 40, y * 40);
				chair_right_black.ShowBitmap();
				break;
			case 13:
				office_chair_down_black.SetTopLeft(x * 40, y * 40);
				office_chair_down_black.ShowBitmap();
				break;
			case 14:
				office_chair_left_black.SetTopLeft(x * 40, y * 40);
				office_chair_left_black.ShowBitmap();
				break;
			case 15:
				office_chair_up_black.SetTopLeft(x * 40, y * 40);
				office_chair_up_black.ShowBitmap();
				break;
			case 16:
				office_chair_right_black.SetTopLeft(x * 40, y * 40);
				office_chair_right_black.ShowBitmap();
				break;
			case 17:
				sofa_down_1_black.SetTopLeft(x * 40, y * 40);
				sofa_down_1_black.ShowBitmap();
				break;
			case 18:
				sofa_left_1_black.SetTopLeft(x * 40, y * 40);
				sofa_left_1_black.ShowBitmap();
				break;
			case 19:
				sofa_up_1_black.SetTopLeft(x * 40, y * 40);
				sofa_up_1_black.ShowBitmap();
				break;
			case 20:
				sofa_right_1_black.SetTopLeft(x * 40, y * 40);
				sofa_right_1_black.ShowBitmap();
				break;
			case 21:
				sofa_down_2_black.SetTopLeft(x * 40, y * 40);
				sofa_down_2_black.ShowBitmap();
				break;
			case 22:
				sofa_left_2_black.SetTopLeft(x * 40, y * 40);
				sofa_left_2_black.ShowBitmap();
				break;
			case 23:
				sofa_up_2_black.SetTopLeft(x * 40, y * 40);
				sofa_up_2_black.ShowBitmap();
				break;
			case 24:
				sofa_right_2_black.SetTopLeft(x * 40, y * 40);
				sofa_right_2_black.ShowBitmap();
				break;
			case 25:
				solosofa_down_1_black.SetTopLeft(x * 40, y * 40);
				solosofa_down_1_black.ShowBitmap();
				break;
			case 26:
				solosofa_left_1_black.SetTopLeft(x * 40, y * 40);
				solosofa_left_1_black.ShowBitmap();
				break;
			case 27:
				solosofa_up_1_black.SetTopLeft(x * 40, y * 40);
				solosofa_up_1_black.ShowBitmap();
				break;
			case 28:
				solosofa_right_1_black.SetTopLeft(x * 40, y * 40);
				solosofa_right_1_black.ShowBitmap();
				break;
			case 29:
				solosofa_down_2_black.SetTopLeft(x * 40, y * 40);
				solosofa_down_2_black.ShowBitmap();
				break;
			case 30:
				solosofa_left_2_black.SetTopLeft(x * 40, y * 40);
				solosofa_left_2_black.ShowBitmap();
				break;
			case 31:
				solosofa_up_2_black.SetTopLeft(x * 40, y * 40);
				solosofa_up_2_black.ShowBitmap();
				break;
			case 32:
				solosofa_right_2_black.SetTopLeft(x * 40, y * 40);
				solosofa_right_2_black.ShowBitmap();
				break;
			case 33:
				circle_H_black.SetTopLeft(x * 40, y * 40);
				circle_H_black.ShowBitmap();
				break;
			case 34:
				circle_L_black.SetTopLeft(x * 40, y * 40);
				circle_L_black.ShowBitmap();
				break;
			case 35:
				rectangle_H_black.SetTopLeft(x * 40, y * 40);
				rectangle_H_black.ShowBitmap();
				break;
			case 36:
				rectangle_L_black.SetTopLeft(x * 40, y * 40);
				rectangle_L_black.ShowBitmap();
				break;
			case 37:
				square_down_black.SetTopLeft(x * 40, y * 40);
				square_down_black.ShowBitmap();
				break;
			case 38:
				square_left_black.SetTopLeft(x * 40, y * 40);
				square_left_black.ShowBitmap();
				break;
			case 39:
				square_up_black.SetTopLeft(x * 40, y * 40);
				square_up_black.ShowBitmap();
				break;
			case 40:
				square_right_black.SetTopLeft(x * 40, y * 40);
				square_right_black.ShowBitmap();
				break;
			case 41:
				TVtable_down_black.SetTopLeft(x * 40, y * 40);
				TVtable_down_black.ShowBitmap();
				break;
			case 42:
				TVtable_left_black.SetTopLeft(x * 40, y * 40);
				TVtable_left_black.ShowBitmap();
				break;
			case 43:
				TVtable_up_black.SetTopLeft(x * 40, y * 40);
				TVtable_up_black.ShowBitmap();
				break;
			case 44:
				TVtable_right_black.SetTopLeft(x * 40, y * 40);
				TVtable_right_black.ShowBitmap();
				break;
			case 45:
				weed_1_black.SetTopLeft(x * 40, y * 40);
				weed_1_black.ShowBitmap();
				break;
			case 46:
				weed_2_black.SetTopLeft(x * 40, y * 40);
				weed_2_black.ShowBitmap();
				break;

			default:
				break;
			}
		}
	}
	void Furniture::LoadBitmap()
	{
		bed_down.LoadBitmap("Bitmaps/Furniture/bed_down.bmp", RGB(255, 255, 255));
		bed_left.LoadBitmap("Bitmaps/Furniture/bed_left.bmp", RGB(255, 255, 255));
		bed_right.LoadBitmap("Bitmaps/Furniture/bed_right.bmp", RGB(255, 255, 255));
		bed_up.LoadBitmap("Bitmaps/Furniture/bed_up.bmp", RGB(255, 255, 255));
		double_bed_down.LoadBitmap("Bitmaps/Furniture/double_bed_down.bmp", RGB(255, 255, 255));
		double_bed_up.LoadBitmap("Bitmaps/Furniture/double_bed_up.bmp", RGB(255, 255, 255));
		double_bed_left.LoadBitmap("Bitmaps/Furniture/double_bed_left.bmp", RGB(255, 255, 255));
		double_bed_right.LoadBitmap("Bitmaps/Furniture/double_bed_right.bmp", RGB(255, 255, 255));
		chair_down.LoadBitmap("Bitmaps/Furniture/chair_down.bmp", RGB(255, 255, 255));
		chair_up.LoadBitmap("Bitmaps/Furniture/chair_up.bmp", RGB(255, 255, 255));
		chair_left.LoadBitmap("Bitmaps/Furniture/chair_left.bmp", RGB(255, 255, 255));
		chair_right.LoadBitmap("Bitmaps/Furniture/chair_right.bmp", RGB(255, 255, 255));
		office_chair_down.LoadBitmap("Bitmaps/Furniture/office_chair_down.bmp", RGB(255, 255, 255));
		office_chair_up.LoadBitmap("Bitmaps/Furniture/office_chair_up.bmp", RGB(255, 255, 255));
		office_chair_left.LoadBitmap("Bitmaps/Furniture/office_chair_left.bmp", RGB(255, 255, 255));
		office_chair_right.LoadBitmap("Bitmaps/Furniture/office_chair_right.bmp", RGB(255, 255, 255));
		sofa_down_1.LoadBitmap("Bitmaps/Furniture/sofa_down_1.bmp", RGB(255, 255, 255));
		sofa_up_1.LoadBitmap("Bitmaps/Furniture/sofa_up_1.bmp", RGB(255, 255, 255));
		sofa_left_1.LoadBitmap("Bitmaps/Furniture/sofa_left_1.bmp", RGB(255, 255, 255));
		sofa_right_1.LoadBitmap("Bitmaps/Furniture/sofa_right_1.bmp", RGB(255, 255, 255));
		sofa_down_2.LoadBitmap("Bitmaps/Furniture/sofa_down_2.bmp", RGB(255, 255, 255));
		sofa_up_2.LoadBitmap("Bitmaps/Furniture/sofa_up_2.bmp", RGB(255, 255, 255));
		sofa_left_2.LoadBitmap("Bitmaps/Furniture/sofa_left_2.bmp", RGB(255, 255, 255));
		sofa_right_2.LoadBitmap("Bitmaps/Furniture/sofa_right_2.bmp", RGB(255, 255, 255));
		circle_H.LoadBitmap("Bitmaps/Furniture/circle_H.bmp", RGB(255, 255, 255));
		circle_L.LoadBitmap("Bitmaps/Furniture/circle_L.bmp", RGB(255, 255, 255));
		rectangle_H.LoadBitmap("Bitmaps/Furniture/rectangle_H.bmp", RGB(255, 255, 255));
		rectangle_L.LoadBitmap("Bitmaps/Furniture/rectangle_L.bmp", RGB(255, 255, 255));
		solosofa_down_1.LoadBitmap("Bitmaps/Furniture/solosofa_down_1.bmp", RGB(255, 255, 255));
		solosofa_up_1.LoadBitmap("Bitmaps/Furniture/solosofa_up_1.bmp", RGB(255, 255, 255));
		solosofa_left_1.LoadBitmap("Bitmaps/Furniture/solosofa_left_1.bmp", RGB(255, 255, 255));
		solosofa_right_1.LoadBitmap("Bitmaps/Furniture/solosofa_right_1.bmp", RGB(255, 255, 255));
		solosofa_down_2.LoadBitmap("Bitmaps/Furniture/solosofa_down_2.bmp", RGB(255, 255, 255));
		solosofa_left_2.LoadBitmap("Bitmaps/Furniture/solosofa_left_2.bmp", RGB(255, 255, 255));
		solosofa_up_2.LoadBitmap("Bitmaps/Furniture/solosofa_up_2.bmp", RGB(255, 255, 255));
		solosofa_right_2.LoadBitmap("Bitmaps/Furniture/solosofa_right_2.bmp", RGB(255, 255, 255));
		square_down.LoadBitmap("Bitmaps/Furniture/square_down.bmp", RGB(255, 255, 255));
		square_up.LoadBitmap("Bitmaps/Furniture/square_up.bmp", RGB(255, 255, 255));
		square_left.LoadBitmap("Bitmaps/Furniture/square_left.bmp", RGB(255, 255, 255));
		square_right.LoadBitmap("Bitmaps/Furniture/square_right.bmp", RGB(255, 255, 255));
		TVtable_down.LoadBitmap("Bitmaps/Furniture/TVtable_down.bmp", RGB(255, 255, 255));
		TVtable_up.LoadBitmap("Bitmaps/Furniture/TVtable_up.bmp", RGB(255, 255, 255));
		TVtable_left.LoadBitmap("Bitmaps/Furniture/TVtable_left.bmp", RGB(255, 255, 255));
		TVtable_right.LoadBitmap("Bitmaps/Furniture/TVtable_right.bmp", RGB(255, 255, 255));
		weed_1.LoadBitmap("Bitmaps/Furniture/weed_1.bmp", RGB(255, 255, 255));
		weed_2.LoadBitmap("Bitmaps/Furniture/weed_2.bmp", RGB(255, 255, 255));


		bed_down_black.LoadBitmap("Bitmaps/Furniture/bed_down_black.bmp", RGB(255, 255, 255));
		bed_left_black.LoadBitmap("Bitmaps/Furniture/bed_left_black.bmp", RGB(255, 255, 255));
		bed_right_black.LoadBitmap("Bitmaps/Furniture/bed_right_black.bmp", RGB(255, 255, 255));
		bed_up_black.LoadBitmap("Bitmaps/Furniture/bed_up_black.bmp", RGB(255, 255, 255));
		double_bed_down_black.LoadBitmap("Bitmaps/Furniture/double_bed_down_black.bmp", RGB(255, 255, 255));
		double_bed_up_black.LoadBitmap("Bitmaps/Furniture/double_bed_up_black.bmp", RGB(255, 255, 255));
		double_bed_left_black.LoadBitmap("Bitmaps/Furniture/double_bed_left_black.bmp", RGB(255, 255, 255));
		double_bed_right_black.LoadBitmap("Bitmaps/Furniture/double_bed_right_black.bmp", RGB(255, 255, 255));
		chair_down_black.LoadBitmap("Bitmaps/Furniture/chair_down_black.bmp", RGB(255, 255, 255));
		chair_up_black.LoadBitmap("Bitmaps/Furniture/chair_up_black.bmp", RGB(255, 255, 255));
		chair_left_black.LoadBitmap("Bitmaps/Furniture/chair_left_black.bmp", RGB(255, 255, 255));
		chair_right_black.LoadBitmap("Bitmaps/Furniture/chair_right_black.bmp", RGB(255, 255, 255));
		office_chair_down_black.LoadBitmap("Bitmaps/Furniture/office_chair_down_black.bmp", RGB(255, 255, 255));
		office_chair_up_black.LoadBitmap("Bitmaps/Furniture/office_chair_up_black.bmp", RGB(255, 255, 255));
		office_chair_left_black.LoadBitmap("Bitmaps/Furniture/office_chair_left_black.bmp", RGB(255, 255, 255));
		office_chair_right_black.LoadBitmap("Bitmaps/Furniture/office_chair_right_black.bmp", RGB(255, 255, 255));
		sofa_down_1_black.LoadBitmap("Bitmaps/Furniture/sofa_down_1_black.bmp", RGB(255, 255, 255));
		sofa_up_1_black.LoadBitmap("Bitmaps/Furniture/sofa_up_1_black.bmp", RGB(255, 255, 255));
		sofa_left_1_black.LoadBitmap("Bitmaps/Furniture/sofa_left_1_black.bmp", RGB(255, 255, 255));
		sofa_right_1_black.LoadBitmap("Bitmaps/Furniture/sofa_right_1_black.bmp", RGB(255, 255, 255));
		sofa_down_2_black.LoadBitmap("Bitmaps/Furniture/sofa_down_2_black.bmp", RGB(255, 255, 255));
		sofa_up_2_black.LoadBitmap("Bitmaps/Furniture/sofa_up_2_black.bmp", RGB(255, 255, 255));
		sofa_left_2_black.LoadBitmap("Bitmaps/Furniture/sofa_left_2_black.bmp", RGB(255, 255, 255));
		sofa_right_2_black.LoadBitmap("Bitmaps/Furniture/sofa_right_2_black.bmp", RGB(255, 255, 255));
		circle_H_black.LoadBitmap("Bitmaps/Furniture/circle_H_black.bmp", RGB(255, 255, 255));
		circle_L_black.LoadBitmap("Bitmaps/Furniture/circle_L_black.bmp", RGB(255, 255, 255));
		rectangle_H_black.LoadBitmap("Bitmaps/Furniture/rectangle_H_black.bmp", RGB(255, 255, 255));
		rectangle_L_black.LoadBitmap("Bitmaps/Furniture/rectangle_L_black.bmp", RGB(255, 255, 255));
		solosofa_down_1_black.LoadBitmap("Bitmaps/Furniture/solosofa_down_1_black.bmp", RGB(255, 255, 255));
		solosofa_up_1_black.LoadBitmap("Bitmaps/Furniture/solosofa_up_1_black.bmp", RGB(255, 255, 255));
		solosofa_left_1_black.LoadBitmap("Bitmaps/Furniture/solosofa_left_1_black.bmp", RGB(255, 255, 255));
		solosofa_right_1_black.LoadBitmap("Bitmaps/Furniture/solosofa_right_1_black.bmp", RGB(255, 255, 255));
		solosofa_down_2_black.LoadBitmap("Bitmaps/Furniture/solosofa_down_2_black.bmp", RGB(255, 255, 255));
		solosofa_left_2_black.LoadBitmap("Bitmaps/Furniture/solosofa_left_2_black.bmp", RGB(255, 255, 255));
		solosofa_up_2_black.LoadBitmap("Bitmaps/Furniture/solosofa_up_2_black.bmp", RGB(255, 255, 255));
		solosofa_right_2_black.LoadBitmap("Bitmaps/Furniture/solosofa_right_2_black.bmp", RGB(255, 255, 255));
		square_down_black.LoadBitmap("Bitmaps/Furniture/square_down_black.bmp", RGB(255, 255, 255));
		square_up_black.LoadBitmap("Bitmaps/Furniture/square_up_black.bmp", RGB(255, 255, 255));
		square_left_black.LoadBitmap("Bitmaps/Furniture/square_left_black.bmp", RGB(255, 255, 255));
		square_right_black.LoadBitmap("Bitmaps/Furniture/square_right_black.bmp", RGB(255, 255, 255));
		TVtable_down_black.LoadBitmap("Bitmaps/Furniture/TVtable_down_black.bmp", RGB(255, 255, 255));
		TVtable_up_black.LoadBitmap("Bitmaps/Furniture/TVtable_up_black.bmp", RGB(255, 255, 255));
		TVtable_left_black.LoadBitmap("Bitmaps/Furniture/TVtable_left_black.bmp", RGB(255, 255, 255));
		TVtable_right_black.LoadBitmap("Bitmaps/Furniture/TVtable_right_black.bmp", RGB(255, 255, 255));
		weed_1_black.LoadBitmap("Bitmaps/Furniture/weed_1_black.bmp", RGB(255, 255, 255));
		weed_2_black.LoadBitmap("Bitmaps/Furniture/weed_2_black.bmp", RGB(255, 255, 255));
	}
	void Furniture::SetIsSaw(bool flag)
	{
		isSaw = flag;
	}
}