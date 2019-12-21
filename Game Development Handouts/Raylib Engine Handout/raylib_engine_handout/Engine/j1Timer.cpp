// ----------------------------------------------------
// j1Timer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "j1Timer.h"

#include "raylib.h"

j1Timer::j1Timer()
{
	Start();
}

void j1Timer::Start()
{
	started_at = GetTime();
}

double j1Timer::ReadSec() const
{
	return (GetTime() - started_at);
}