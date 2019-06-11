

#ifndef WEAPON_H
#define WEAPON_H
namespace game_framework
{

	class Weapon
	{
	public:
		void Init();
		Weapon(int,int,int);
		virtual ~Weapon();
		virtual void Fire() {}
		int GetDamage();
	protected:
		int _damage;
		int _ammon;
		int _sound_count_start;
		int _sound_count;
	};
	class HK416 :public Weapon
	{
	public:
		HK416();
		~HK416();
		void Fire();
		
	private:
	};
	class P9 :public Weapon
	{
	public:
		P9();
		~P9();
		virtual void Fire();

	private:
	};
	

}

#endif // !WEAPON_H