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
	Weapon::Weapon(int damage, int ammon,int sound_start) :_damage(damage), _ammon(ammon),_sound_count_start(sound_start)	//創立武器 武器(傷害值,彈夾量,聲音持續時間)
	{
		gun_sound_count += 4;
		_sound_count = _sound_count_start;
	}

	Weapon::~Weapon()
	{
		TRACE("~Weapon()\n");
	}
	int Weapon::GetDamage()	//取得此槍枝的傷害
	{
		return _damage;		//回傳此槍枝傷害
	}
	HK416::HK416():Weapon(33,30, gun_sound_count)	//建立槍枝HK416 (傷害值33,彈夾量30,聲音值固定100)
	{
		for(int i= _sound_count_start;i< _sound_count_start+4;i++)		//持續發送聲音
			CAudio::Instance()->Load(i, "Sounds\\AR15_gun_sound.mp3");	//取得聲音源再發送聲音
		TRACE("HK416:%d %d\n", _sound_count_start, _sound_count);		//DeBug
	}

	void HK416::Fire()	//HK416開火
	{
		CAudio::Instance()->Play(_sound_count, false);
		_sound_count++;
		if (_sound_count > _sound_count_start + 3)
		{
			_sound_count = _sound_count_start;
		}
	}
	
	HK416::~HK416() { TRACE("~HK416()\n"); }

	P9::P9() :Weapon(20, 12, gun_sound_count)	//建立槍枝P9 (傷害值20,彈夾量12,聲音值固定100)
	{	
		for (int i = _sound_count_start;i < _sound_count_start + 4;i++)	//持續發送聲音
			CAudio::Instance()->Load(i, "Sounds\\P9.mp3");				//取得聲音源再發送聲音
		TRACE("P9:%d %d\n", _sound_count_start, _sound_count);			//DeBug
	}
	void P9::Fire()//P9開火
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