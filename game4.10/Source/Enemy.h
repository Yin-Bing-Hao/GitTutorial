#ifndef ENEMEY_H
#define ENEMEY_H
namespace game_framework {
	class Enemy
	{
	public:
		Enemy(int x, int y,int dir);
		//Enemy(int x, int y, Weapon); //�ݧ�sweapon class
		~Enemy();
		void OnMove();
		void OnShow();
		void LoadBitmap();
		void Initialize();
		int GetIndexX();
		int GetIndexY();
		void SetIsSaw(bool flag);
		void GetHurt(int);
		int GetLifePoint();
		void searchEnemy(CGameMap* map, vector<Soldier*>& players);
		void attackPlayer();
		void shoot();
	private:
		CMovingBitmap enemy_U, enemy_RU, enemy_R, enemy_RD, enemy_D, enemy_LD, enemy_L, enemy_LU;
		CMovingBitmap enemyRUD, enemyLUD, enemyRDD, enemyLDD, enemyRUU, enemyLUU, enemyRDU, enemyLDU;
		int lifePoint;
		int x, y;					// �H�����W���y��
		int shoot_count_time, reaction_time;
		int index_x, index_y;
		int moving_index_x, moving_index_y;
		int direction,init_way;				//�ݪ���V
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
		bool isSaw;						//�O�_�Q�ݨ�
		Soldier* target;
		int lock_enemy;
		Weapon *weapon;
	};
}
#endif // !ENEMEY_H


