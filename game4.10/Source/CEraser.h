#ifndef SOLIDER
#define SOLIDER


namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���ѥi�H����L�ηƹ�������l
	// �����N�i�H��g���ۤv���{���F
	/////////////////////////////////////////////////////////////////////////////
	class CGameMap;
	class CEraser
	{
	public:
		CEraser();
		int  GetX1();					// ���l���W�� x �y��
		int  GetY1();					// ���l���W�� y �y��
		int  GetX2();					// ���l�k�U�� x �y��
		int  GetY2();					// ���l�k�U�� y �y��
		int	GetIndexX();
		int GetIndexY();
		void Initialize();				// �]�w���l����l��
		void LoadBitmap();				// ���J�ϧ�
		void OnMove();					// �������l
		void OnShow();					// �N���l�ϧζK��e��
		void MoveLeftIndex();
		void MoveRightIndex();
		void MoveUpIndex();
		void MoveDownIndex();
		void SetMovingDown(bool flag);	// �]�w�O�_���b���U����
		void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
		void SetMovingRight(bool flag); // �]�w�O�_���b���k����
		void SetMovingUp(bool flag);	// �]�w�O�_���b���W����
		void SetMovingRightDown(bool flag);	// �]�w�O�_���b���U����
		void SetMovingLeftDown(bool flag);	// �]�w�O�_���b��������
		void SetMovingRightUp(bool flag); // �]�w�O�_���b���k����
		void SetMovingLeftUp(bool flag);	// �]�w�O�_���b���W����
		void SetWatchLeft(bool flag);		//�]�w�O�_������
		void SetWatchUp(bool flag);			//�]�w�O�_���W��
		void SetWatchRight(bool flag);		//�]�w�O�_���k��
		void SetWatchDown(bool flag);		//�]�w�O�_���U��
		void SetWatchLeftUp(bool flag);		//�]�w�O�_������
		void SetWatchRightUp(bool flag);	//�]�w�O�_���W��
		void SetWatchRightDown(bool flag);	//�]�w�O�_���k��
		void SetWatchLeftDown(bool flag);	//�]�w�O�_���U��
		void SetChoosen(bool flag);
		void DrawLineFirst(vector<int>::iterator iter,int *line_x,int *line_y);
		void DrawLineSecond(vector<int>::iterator iter,int *line_x,int *line_y);
		void SetXY(int nx, int ny);		// �]�w���l���W���y��
		bool IsChoosen();
		void SetRoadLine(int mouse_x, int mouse_y, CGameMap& map);
		const vector<int>& GetRoadLine();
		const int GetIt();
	protected:
		CAnimation peopleR,peopleL,peopleU,peopleD;		// ���l���ʵe
		CAnimation peopleRU, peopleLU, peopleRD, peopleLD;
		CAnimation breakPoint, lineUP, lineRU, lineRight, lineRD, lineDown, lineLD,lineLeft,lineLU;
		int x, y;					// ���l���W���y��
		int index_x, index_y;
		int moving_index_x, moving_index_y;
		int last;
		bool isMovingDown;			// �O�_���b���U����
		bool isMovingLeft;			// �O�_���b��������
		bool isMovingRight;			// �O�_���b���k����
		bool isMovingUp;			// �O�_���b���W����
		bool isMovingRightDown;			// �O�_���b���k�U����
		bool isMovingLeftDown;			// �O�_���b�����U����
		bool isMovingRightUp;			// �O�_���b���k�W����
		bool isMovingLeftUp;			// �O�_���b�����W����
		bool isWatchDown;				//�O�_���U��
		bool isWatchUp;					//�O�_���W��
		bool isWatchLeft;				//�O�_������
		bool isWatchRight;				//�O�_���k��
		bool isWatchRightDown;			//�O�_���k�U��
		bool isWatchLeftUp;				//�O�_�����W��
		bool isWatchLeftDown;			//�O�_�����U��
		bool isWatchRightUp;			//�O�_���k�W��
		bool isChoosen;
		vector<int> roadLine;
		vector<int>::iterator it;
		vector<vector<int> > line;
		vector<int> lineXY;
	};
}
#endif // !Solider