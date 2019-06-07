#ifndef SOLIDER_H
#define SOLIDER_H


namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���ѥi�H����L�ηƹ�������l
	// �����N�i�H��g���ۤv���{���F
	/////////////////////////////////////////////////////////////////////////////
	class CGameMap;
	class Enemy;
	class Weapon;
	class Line;
	class Soldier
	{
	public:
		Soldier();
		~Soldier();
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
		void MoveL(bool flag, CGameMap* map);
		void MoveR(bool flag, CGameMap* map);
		void MoveU(bool flag, CGameMap* map);
		void MoveD(bool flag, CGameMap* map);
		void MoveLU(bool flag, CGameMap* map);
		void MoveLD(bool flag, CGameMap* map);
		void MoveRU(bool flag, CGameMap* map);
		void MoveRD(bool flag, CGameMap* map);
		void TestNext(CGameMap* map);	//���ժ�484�b����
		void TestInRedLine(CPoint point);
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
		void SetNextDoor(bool flag);		//�]�w�H�b������
		void SetChoosen(bool flag);			//�]�w�H�Q���
		void SetRoadLine(bool flag);		//�]�w�i�e�u
		void SetInRoadLine(bool flag);		//�]�w�u�W�i���ʧ@
		void SetAction(bool flag);			//�]�w���ʧ@
		void DrawLineFirst(vector<Line*>::iterator iter,int *line_x,int *line_y);
		void DrawLineSecond(vector<Line*>::iterator iter,int *line_x,int *line_y);
		void SetXY(int nx, int ny);			// �]�w���l���W���y��
		bool IsChoosen();					//484�Q���
		bool IsSetRoadLine(CPoint &point);	//484�e�u
		bool IsSetAction(CPoint &point);	//484���ʧ@
		bool IsInRoadLine();				//484�I�b�u�W
		bool GetIsMoveNext();				
		void SetRoadLine(int mouse_x, int mouse_y, CGameMap* map);
		void ChangeGun(int);
		const vector<Line*>& GetRoadLine();
		const int GetWay();
		void searchEnemy(CGameMap* map,vector<Enemy*>& enemys); //�j�M�ĤH
		void attackEnemy();
		void shoot();
	protected:
		void linearSearch(Enemy* _target, double dx, double dy, CGameMap* map, vector<Enemy*>& enemys);
		CAnimation peopleR,peopleL,peopleU,peopleD;		// ���l���ʵe
		CAnimation peopleRU, peopleLU, peopleRD, peopleLD;
		CAnimation breakPoint, lineUP, lineRU, lineRight, lineRD, lineDown, lineLD,lineLeft,lineLU;
		CMovingBitmap HK416_Photo,P9_Photo;
		Weapon *equipment[4];
		int x, y;					// ���l���W���y��
		int index_x, index_y;
		int moving_index_x, moving_index_y;
		int x1, x2, y1, y2;
		int x11, x12, x21, x22, y11, y12, y21, y22;
		int end_x, end_y ;
		int direction,gun;
		bool isMoveNextIndex;		//�O�_���ʨ�U�@���m
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
		bool isNextDoor;
		bool isChoosen;
		bool isSetRoadLine;
		bool isSetAction;
		bool isInRoadLine;
		vector<Line*> roadLine;
		vector<Line*>::iterator way;
		vector<vector<int> > line;
		vector<int> lineXY;
		Enemy *target;
		Weapon *weapon;

	};
	
}
#endif // !Solider_H