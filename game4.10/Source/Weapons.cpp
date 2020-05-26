#include "stdafx.h"
#include "Resource.h"
#include "MainFrm.h"
#include "gameDoc.h"
#include <mmsystem.h>
#include <windows.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Weapons.h"
#include "mygame.h"
#include <thread>

namespace game_framework
{
	static int gun_sound_count = 100;
	Weapon::Weapon(int damage, int ammon,int sound_start) :_damage(damage), _ammon(ammon),_sound_count_start(sound_start)	//�ХߪZ�� �Z��(�ˮ`��,�u���q,�n������ɶ�)
	{
		gun_sound_count += 4;
		_sound_count = _sound_count_start;
	}

	Weapon::~Weapon()
	{
		TRACE("~Weapon()\n");
	}
	int Weapon::GetDamage()	//���o���j�K���ˮ`
	{
		return _damage;		//�^�Ǧ��j�K�ˮ`
	}
	HK416::HK416():Weapon(33,30, gun_sound_count)	//�إߺj�KHK416 (�ˮ`��33,�u���q30,�n���ȩT�w100)
	{
		for(int i= _sound_count_start;i< _sound_count_start+4;i++)		//����o�e�n��
			CAudio::Instance()->Load(i, "Sounds\\AR15_gun_sound.mp3");	//���o�n�����A�o�e�n��
		TRACE("HK416:%d %d\n", _sound_count_start, _sound_count);		//DeBug
	}

	void HK416::Fire()	//HK416�}��
	{
		CAudio::Instance()->Play(_sound_count, false);
		_sound_count++;
		if (_sound_count > _sound_count_start + 3)
		{
			_sound_count = _sound_count_start;
		}
	}
	
	HK416::~HK416() { TRACE("~HK416()\n"); }

	P9::P9() :Weapon(20, 12, gun_sound_count)	//�إߺj�KP9 (�ˮ`��20,�u���q12,�n���ȩT�w100)
	{	
		for (int i = _sound_count_start;i < _sound_count_start + 4;i++)	//����o�e�n��
			CAudio::Instance()->Load(i, "Sounds\\P9.mp3");				//���o�n�����A�o�e�n��
		TRACE("P9:%d %d\n", _sound_count_start, _sound_count);			//DeBug
	}
	void P9::Fire()//P9�}��
	{
		CAudio::Instance()->Play(_sound_count, false);
		_sound_count++;
		if (_sound_count > _sound_count_start+3)
		{
			_sound_count = _sound_count_start;
		}
	}

	P9::~P9() { TRACE("~P9()\n"); }
}