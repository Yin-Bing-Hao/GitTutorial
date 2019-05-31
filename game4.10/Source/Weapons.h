

#ifndef WEAPON_H
#define WEAPON_H
namespace game_framework
{

	class Weapon
	{
	public:
		Weapon(int, int);
		virtual ~Weapon();
		virtual void Fire() {}
		int GetDamage();
	protected:
		int _damage;
		int _ammon;
	};
	class HK416 :public Weapon
	{
	public:
		HK416();
		~HK416();
		virtual void Fire();
		
	private:
	};
	

}

#endif // !WEAPON_H