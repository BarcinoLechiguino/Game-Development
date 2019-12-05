#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2Point.h"
#include "UI.h"
#include "UI_Image.h"
#include "UI_Text.h"

class UI;

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes

//class j1UI : public j1Gui {};
//class j1UiImage : public j1UI {};
//class j1UiText : public j1UI {};

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions

	const SDL_Texture* GetAtlas() const;

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;
};

#endif // __j1GUI_H__