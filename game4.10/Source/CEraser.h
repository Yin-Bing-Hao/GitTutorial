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
		void SetPeopleChioce(bool flag);
		void SetXY(int nx, int ny);		// 設定擦子左上角座標
<<<<<<< HEAD
		void SetRoadLine(int mouse_x,int mouse_y);
		void ShowRoadLine();
		void SetChoosen(bool choosen);
		bool GetChoosen();
	protected:
		vector<int> roadLine;
		CAnimation animation;		// 擦子的動畫
=======
		bool isPeopleChoice();
	protected:
		CAnimation animation,animation2;		// 擦子的動畫
>>>>>>> c8bd7f0bdaa1728f5c51dab00eaf9311ddd2ed98
		int x, y;					// 擦子左上角座標
		int index_x, index_y;
		int moving_index_x, moving_index_y;
		bool isMovingDown;			// 是否正在往下移動
		bool isMovingLeft;			// 是否正在往左移動
		bool isMovingRight;			// 是否正在往右移動
		bool isMovingUp;			// 是否正在往上移動
<<<<<<< HEAD
		bool isChoosen;
=======
		bool isChoice;
>>>>>>> c8bd7f0bdaa1728f5c51dab00eaf9311ddd2ed98
	};
}