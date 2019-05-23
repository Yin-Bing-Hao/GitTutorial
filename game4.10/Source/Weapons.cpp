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
	HK416::HK416():Weapon(33,30)
	{
		CAudio::Instance()->Load(AUDIO_HK416_1, "Sounds/AR15_gun_sound.mp3");
		CAudio::Instance()->Load(AUDIO_HK416_2, "Sounds/AR15_gun_sound.mp3");
		CAudio::Instance()->Load(AUDIO_HK416_3, "Sounds/AR15_gun_sound.mp3");
		CAudio::Instance()->Load(AUDIO_HK416_4, "Sounds/AR15_gun_sound.mp3");

	}
	void HK416::Fire()
	{
		static int play_ID = AUDIO_HK416_1;
		CAudio::Instance()->Play(play_ID, false);
		play_ID++;
		if (play_ID > AUDIO_HK416_4)
		{
			play_ID = AUDIO_HK416_1;
		}
	}
	
	HK416::~HK416() { TRACE("~HK416()\n"); }
}