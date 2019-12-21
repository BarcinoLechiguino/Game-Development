#ifndef __j1WINDOW_H__
#define __j1WINDOW_H__

#include "j1Module.h"

enum j1WindowEvent
{
    WINDOW_EVENT_QUIT = 0,
    WINDOW_EVENT_HIDE = 1,
    WINDOW_EVENT_SHOW = 2,
    WINDOW_EVENT_RESIZE = 3,
    WINDOW_EVENT_COUNT       // Max window events
};

class j1Window : public j1Module
{
public:

	j1Window();

	// Destructor
	virtual ~j1Window();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

    // Called each loop iteration
    bool PreUpdate();

	// Changae title
	void SetTitle(const char* new_title);

	// Retrieve window size
    uint GetWidth() const;
    uint GetHeight() const;

	// Retrieve window scale
	uint GetScale() const;

    // Gather relevant win events
    bool GetWindowEvent(j1WindowEvent ev);

private:
	p2SString	title;
	uint		width;
	uint		height;
	uint		scale;

    bool windowEvents[WINDOW_EVENT_COUNT];
};

#endif // __j1WINDOW_H__