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

namespace game_framework
{
	Weapon::Weapon(int damage, int ammon) :_damage(damage), _ammon(ammon) {}

	Weapon::~Weapon()
	{
	}
	HK416::HK416():Weapon(33,30){}
	HK416::~HK416() { delete this; }
}