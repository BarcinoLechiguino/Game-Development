#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "j1Textures.h"
#include "p2List.h"
#include "UI.h"
#include "UI_Image.h"
#include "UI_Text.h"

//class UI;

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes

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

public:
	/*const*/ SDL_Texture* GetAtlas() const;
	
	// TODO 2: Create the factory methods
	// Gui creation functions
	UI* CreateElement(UI_Element element, int x, int y, SDL_Rect* rect = nullptr, p2SString* string = nullptr); //This might cause a problem when the element is RELEASED
	
	void Debug_UI();
	bool ui_debug;

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;

	p2List<UI*> elements;		//List where all the UI elements in a scene will be stored at.
};

#endif // __j1GUI_H__