#ifndef ENEMEY_H
#define ENEMEY_H

namespace game_framework {
	class Enemy
	{
	public:
		void OnMove();
		void OnShow();
		void LoadBitmap();
		void Initialize();

	private:
		int lifePoint;
		int x, y;					// �H�����W���y��
		int index_x, index_y;
		int moving_index_x, moving_index_y;
		int direction;				//�ݪ���V
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
	};
}

#endif // !ENEMEY_H


