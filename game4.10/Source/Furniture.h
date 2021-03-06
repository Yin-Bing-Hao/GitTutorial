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
		void SetIsSaw(bool flag);
		int GetX();
		int GetY();
	private:
		int x;
		int y;
		int type;
		bool isSaw;
		CMovingBitmap bed_down, bed_left, bed_right, bed_up, double_bed_down, double_bed_up, double_bed_left, double_bed_right;
		CMovingBitmap chair_down, chair_up, chair_left, chair_right, office_chair_down, office_chair_up, office_chair_left, office_chair_right;
		CMovingBitmap sofa_down_1, sofa_up_1, sofa_left_1, sofa_right_1, sofa_down_2, sofa_up_2, sofa_left_2, sofa_right_2;
		CMovingBitmap circle_H, circle_L, rectangle_H, rectangle_L;
		CMovingBitmap solosofa_down_1, solosofa_up_1, solosofa_left_1, solosofa_right_1, solosofa_down_2, solosofa_left_2, solosofa_up_2, solosofa_right_2;
		CMovingBitmap square_down, square_up, square_left, square_right;
		CMovingBitmap TVtable_down, TVtable_up, TVtable_left, TVtable_right;
		CMovingBitmap weed_1, weed_2;
		CMovingBitmap bed_down_black, bed_left_black, bed_right_black, bed_up_black, double_bed_down_black, double_bed_up_black, double_bed_left_black, double_bed_right_black;
		CMovingBitmap chair_down_black, chair_up_black, chair_left_black, chair_right_black, office_chair_down_black, office_chair_up_black, office_chair_left_black, office_chair_right_black;
		CMovingBitmap sofa_down_1_black, sofa_up_1_black, sofa_left_1_black, sofa_right_1_black, sofa_down_2_black, sofa_up_2_black, sofa_left_2_black, sofa_right_2_black;
		CMovingBitmap circle_H_black, circle_L_black, rectangle_H_black, rectangle_L_black;
		CMovingBitmap solosofa_down_1_black, solosofa_up_1_black, solosofa_left_1_black, solosofa_right_1_black, solosofa_down_2_black, solosofa_left_2_black, solosofa_up_2_black, solosofa_right_2_black;
		CMovingBitmap square_down_black, square_up_black, square_left_black, square_right_black;
		CMovingBitmap TVtable_down_black, TVtable_up_black, TVtable_left_black, TVtable_right_black;
		CMovingBitmap weed_1_black, weed_2_black;
	};
}
#endif 
