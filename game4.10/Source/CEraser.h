#ifndef SOLIDER
#define SOLIDER


namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 這個class提供可以用鍵盤或滑鼠控制的擦子
	// 看懂就可以改寫成自己的程式了
	/////////////////////////////////////////////////////////////////////////////
	class CGameMap;
	class CEraser
	{
	public:
		CEraser();
		int  GetX1();					// 擦子左上角 x 座標
		int  GetY1();					// 擦子左上角 y 座標
		int  GetX2();					// 擦子右下角 x 座標
		int  GetY2();					// 擦子右下角 y 座標
		int	GetIndexX();
		int GetIndexY();
		void Initialize();				// 設定擦子為初始值
		void LoadBitmap();				// 載入圖形
		void OnMove();					// 移動擦子
		void OnShow();					// 將擦子圖形貼到畫面
		void MoveLeftIndex();
		void MoveRightIndex();
		void MoveUpIndex();
		void MoveDownIndex();
		void SetMovingDown(bool flag);	// 設定是否正在往下移動
		void SetMovingLeft(bool flag);	// 設定是否正在往左移動
		void SetMovingRight(bool flag); // 設定是否正在往右移動
		void SetMovingUp(bool flag);	// 設定是否正在往上移動
		void SetMovingRightDown(bool flag);	// 設定是否正在往下移動
		void SetMovingLeftDown(bool flag);	// 設定是否正在往左移動
		void SetMovingRightUp(bool flag); // 設定是否正在往右移動
		void SetMovingLeftUp(bool flag);	// 設定是否正在往上移動
		void SetChoosen(bool flag);
		void SetXY(int nx, int ny);		// 設定擦子左上角座標
		bool IsChoosen();
		void SetRoadLine(int mouse_x, int mouse_y, CGameMap& map);
		const vector<int>& GetRoadLine();
		const int GetIt();
	protected:
		CAnimation animation,animation2;		// 擦子的動畫
		int x, y;					// 擦子左上角座標
		int index_x, index_y;
		int moving_index_x, moving_index_y;
		bool isMovingDown;			// 是否正在往下移動
		bool isMovingLeft;			// 是否正在往左移動
		bool isMovingRight;			// 是否正在往右移動
		bool isMovingUp;			// 是否正在往上移動
		bool isMovingRightDown;			// 是否正在往右下移動
		bool isMovingLeftDown;			// 是否正在往左下移動
		bool isMovingRightUp;			// 是否正在往右上移動
		bool isMovingLeftUp;			// 是否正在往左上移動
		bool isChoosen;
		vector<int> roadLine;
		vector<int>::iterator it;
	};
}
#endif // !Solider