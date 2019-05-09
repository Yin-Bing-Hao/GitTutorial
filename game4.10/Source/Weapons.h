

#ifndef WEAPON_H
#define WEAPON_H
namespace game_framework
{
	class Weapon
	{
	public:
		Weapon(int,int);
		virtual ~Weapon();

	protected:
		int _damage;
		int _ammon;
	};
	class HK416 :public Weapon
	{
	public:
		HK416();
		virtual ~HK416();
	protected:
		int _damage, _ammon;
	private:
	};
	

}

#endif // !WEAPON_H