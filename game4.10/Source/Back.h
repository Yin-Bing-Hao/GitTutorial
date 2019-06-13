#ifndef BACK_H
#define BACK_H
namespace game_framework {
	class Back {
	public:
		Back();
		~Back();
		Back(int NUM, int Lux, int Luy);
		void Initialize();
		void LoadBitmap();
		void OnShow();
		void SetLight(bool flag);
		void SetObject(int num);
	protected:
		CMovingBitmap sand, wood, sidewalk, sand_light, wood_light;
		CMovingBitmap floor_1, floor_2, floor_3, floor_1_Light, floor_2_Light, floor_3_Light;
		bool isLight;
	private:
		int floor, ObjectNum;
		int X, Y;
		int MW, MH;
	};
}
#endif // !Back_H