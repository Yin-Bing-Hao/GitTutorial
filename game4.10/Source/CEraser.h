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
		void SetPeopleChioce(bool flag);
		void SetXY(int nx, int ny);		// �]�w���l���W���y��
<<<<<<< HEAD
		void SetRoadLine(int mouse_x,int mouse_y);
		void ShowRoadLine();
		void SetChoosen(bool choosen);
		bool GetChoosen();
	protected:
		vector<int> roadLine;
		CAnimation animation;		// ���l���ʵe
=======
		bool isPeopleChoice();
	protected:
		CAnimation animation,animation2;		// ���l���ʵe
>>>>>>> c8bd7f0bdaa1728f5c51dab00eaf9311ddd2ed98
		int x, y;					// ���l���W���y��
		int index_x, index_y;
		int moving_index_x, moving_index_y;
		bool isMovingDown;			// �O�_���b���U����
		bool isMovingLeft;			// �O�_���b��������
		bool isMovingRight;			// �O�_���b���k����
		bool isMovingUp;			// �O�_���b���W����
<<<<<<< HEAD
		bool isChoosen;
=======
		bool isChoice;
>>>>>>> c8bd7f0bdaa1728f5c51dab00eaf9311ddd2ed98
	};
}