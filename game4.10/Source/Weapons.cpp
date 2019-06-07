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
	Weapon::Weapon(int damage, int ammon) :_damage(damage), _ammon(ammon) {}

	Weapon::~Weapon()
	{
		TRACE("~Weapon()\n");
	}
	int Weapon::GetDamage()
	{
		return _damage;
	}
	HK416::HK416():Weapon(33,30){}

	void HK416::Fire()
	{
		static int HK416_play_ID = AUDIO_HK416_1;
		CAudio::Instance()->Play(HK416_play_ID, false);
		HK416_play_ID++;
		if (HK416_play_ID > AUDIO_HK416_4)
		{
			HK416_play_ID = AUDIO_HK416_1;
		}
	}
	
	HK416::~HK416() { TRACE("~HK416()\n"); }

	P9::P9() :Weapon(20, 12){}
	void P9::Fire()
	{
		static int play_ID = AUDIO_P9_1;
		CAudio::Instance()->Play(play_ID, false);
		play_ID++;
		if (play_ID > AUDIO_P9_3)
		{
			play_ID = AUDIO_P9_1;
		}
	}

	P9::~P9() { TRACE("~P9()\n"); }
}