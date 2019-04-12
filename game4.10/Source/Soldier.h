#ifndef SOLIDER_H
#define SOLIDER_H


namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 這個class提供可以用鍵盤或滑鼠控制的擦子
	// 看懂就可以改寫成自己的程式了
	/////////////////////////////////////////////////////////////////////////////
	class CGameMap;
	class Soldier
	{
	public:
		Soldier();
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
		void MoveL(bool flag, CGameMap& map);
		void MoveR(bool flag, CGameMap& map);
		void MoveU(bool flag, CGameMap& map);
		void MoveD(bool flag, CGameMap& map);
		void MoveLU(bool flag, CGameMap& map);
		void MoveLD(bool flag, CGameMap& map);
		void MoveRU(bool flag, CGameMap& map);
		void MoveRD(bool flag, CGameMap& map);
		void SetMovingDown(bool flag);	// 設定是否正在往下移動
		void SetMovingLeft(bool flag);	// 設定是否正在往左移動
		void SetMovingRight(bool flag); // 設定是否正在往右移動
		void SetMovingUp(bool flag);	// 設定是否正在往上移動
		void SetMovingRightDown(bool flag);	// 設定是否正在往下移動
		void SetMovingLeftDown(bool flag);	// 設定是否正在往左移動
		void SetMovingRightUp(bool flag); // 設定是否正在往右移動
		void SetMovingLeftUp(bool flag);	// 設定是否正在往上移動
		void SetWatchLeft(bool flag);		//設定是否往左看
		void SetWatchUp(bool flag);			//設定是否往上看
		void SetWatchRight(bool flag);		//設定是否往右看
		void SetWatchDown(bool flag);		//設定是否往下看
		void SetWatchLeftUp(bool flag);		//設定是否往左看
		void SetWatchRightUp(bool flag);	//設定是否往上看
		void SetWatchRightDown(bool flag);	//設定是否往右看
		void SetWatchLeftDown(bool flag);	//設定是否往下看
		void SetChoosen(bool flag);
		void DrawLineFirst(vector<int>::iterator iter,int *line_x,int *line_y);
		void DrawLineSecond(vector<int>::iterator iter,int *line_x,int *line_y);
		void SetXY(int nx, int ny);		// 設定擦子左上角座標
		bool IsChoosen();
		bool GetIsMoveNext();
		void SetRoadLine(int mouse_x, int mouse_y, CGameMap& map);
		const vector<int>& GetRoadLine();
		const int GetWay();
	protected:
		CAnimation peopleR,peopleL,peopleU,peopleD;		// 擦子的動畫
		CAnimation peopleRU, peopleLU, peopleRD, peopleLD;
		CAnimation breakPoint, lineUP, lineRU, lineRight, lineRD, lineDown, lineLD,lineLeft,lineLU;
		int x, y;					// 擦子左上角座標
		int index_x, index_y;
		int moving_index_x, moving_index_y;
		int direction;
		bool isMoveNextIndex;		//是否移動到下一格位置
		bool isMovingDown;			// 是否正在往下移動
		bool isMovingLeft;			// 是否正在往左移動
		bool isMovingRight;			// 是否正在往右移動
		bool isMovingUp;			// 是否正在往上移動
		bool isMovingRightDown;			// 是否正在往右下移動
		bool isMovingLeftDown;			// 是否正在往左下移動
		bool isMovingRightUp;			// 是否正在往右上移動
		bool isMovingLeftUp;			// 是否正在往左上移動
		bool isWatchDown;				//是否往下看
		bool isWatchUp;					//是否往上看
		bool isWatchLeft;				//是否往左看
		bool isWatchRight;				//是否往右看
		bool isWatchRightDown;			//是否往右下看
		bool isWatchLeftUp;				//是否往左上看
		bool isWatchLeftDown;			//是否往左下看
		bool isWatchRightUp;			//是否往右上看
		bool isChoosen;
		vector<int> roadLine;
		vector<int>::iterator way;
		vector<vector<int> > line;
		vector<int> lineXY;
	};
}
#endif // !Solider_H