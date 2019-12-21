#ifndef __j1TIMER_H__
#define __j1TIMER_H__

#include "p2Defs.h"

class j1Timer
{
public:

	// Constructor
	j1Timer();

	void Start();
	double ReadSec() const;

private:

    // Start time in seconds
	double started_at;   
};

#endif //__j1TIMER_H__