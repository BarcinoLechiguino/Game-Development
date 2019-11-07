// ----------------------------------------------------
// j1PerfTimer.cpp
// Slow timer with microsecond precision
// ----------------------------------------------------

#include "j1PerfTimer.h"
#include "SDL\include\SDL_timer.h"

uint64 j1PerfTimer::frequency = 0;

// ---------------------------------------------
j1PerfTimer::j1PerfTimer()
{
	// TODO 2: Fill Constructor, Start(),ReadMs() and ReadTicks() methods
	// they are simple, one line each!
	Start();
}

// ---------------------------------------------
void j1PerfTimer::Start()
{
	started_at = SDL_GetPerformanceCounter(); //Gets the ticks at a given time and sets it as the start of the count.
}

// ---------------------------------------------
double j1PerfTimer::ReadMs() const
{
	return (SDL_GetPerformanceCounter() - started_at) / (SDL_GetPerformanceFrequency() / 1000); //Gets the milliseconds taking into account the start of the count, the current count, and the amount of ticks per second
}

// ---------------------------------------------
uint64 j1PerfTimer::ReadTicks() const
{
	return  SDL_GetPerformanceCounter() - started_at; //Gets the amount of ticks at a given time after assigning a count start.
}


