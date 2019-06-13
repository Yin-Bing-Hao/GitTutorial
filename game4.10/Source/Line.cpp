#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include <vector>
#include <cmath>
#include "Line.h"
#include<iostream>

namespace game_framework {
	Line::Line(unsigned int watch)
	{
		_operation = 0;
		_watch = watch;
		_count = 0;
	}
	unsigned int Line::getWatch()
	{
		return _watch;
	}
	Line::~Line() {}
}