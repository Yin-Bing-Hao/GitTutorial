#ifndef ENEMEY_H
#define ENEMEY_H
namespace game_framework {
	class Enemy
	{
	public:
		Enemy(int x, int y,int dir);
		//Enemy(int x, int y, Weapon); //待更新weapon class
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
		int x, y;					// 人物左上角座標
		int shoot_count_time, reaction_time;
		int index_x, index_y;
		int moving_index_x, moving_index_y;
		int direction,init_way;				//看的方向
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
		bool isSaw;						//是否被看到
		Soldier* target;
		int lock_enemy;
		Weapon *weapon;
	};
}
#endif // !ENEMEY_H


