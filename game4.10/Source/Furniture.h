#ifndef FURNITURE_H
#define FURNITURE_H

namespace game_framework {
	class Furniture
	{
	public:
		Furniture(int x,int y,int type);
		~Furniture();
		void OnShow();
		void LoadBitmap();
	private:
		int x;
		int y;
		int type;
		CMovingBitmap bed_down, bed_left, bed_right, bed_up, double_bed_down, double_bed_up, double_bed_left, double_bed_right;
		CMovingBitmap chair_down, chair_up, chair_left, chair_right, office_chair_down, office_chair_up, office_chair_left, office_chair_right;
		CMovingBitmap sofa_down_1, sofa_up_1, sofa_left_1, sofa_right_1, sofa_down_2, sofa_up_2, sofa_left_2, sofa_right_2;
		CMovingBitmap circle_H, circle_L, rectangle_H, rectangle_L;
		CMovingBitmap solosofa_down_1, solosofa_up_1, solosofa_left_1, solosofa_right_1, solosofa_down_2, solosofa_left_2, solosofa_up_2, solosofa_right_2;
		CMovingBitmap square_down, square_up, square_left, square_right;
		CMovingBitmap TVtable_down, TVtable_up, TVtable_left, TVtable_right;
		CMovingBitmap weed_1, weed_2;
	};
}

#endif 
