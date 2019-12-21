#ifndef __j1INPUT_H__
#define __j1INPUT_H__

#include "j1Module.h"

class j1Input : public j1Module
{
public:

	j1Input();

	// Destructor
	virtual ~j1Input();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before quitting
	bool CleanUp();

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);

    // TODO: Create other functions like the ones provided by raylib,
    // it could be useful to centralize keyboard/mouse/gamepad inputs
    // in this class instead of calling them directly around the code.
};

#endif // __j1INPUT_H__