#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "j1Textures.h"
#include "p2List.h"
#include "UI.h"
#include "UI_Image.h"
#include "UI_Text.h"
#include "UI_Button.h"

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
	UI* CreateElement(UI_Element element, int x, int y, SDL_Rect* idle = nullptr, SDL_Rect* hover = nullptr, SDL_Rect* clicked = nullptr, p2SString* string = nullptr); //This might cause a problem when the element is RELEASED
	UI* CreateImage(UI_Element element, int x, int y, SDL_Rect rect, UI_Image* imgCallback, UI* parent = nullptr);
	UI* CreateText(UI_Element element, int x, int y, SDL_Rect hitbox, _TTF_Font* font, SDL_Color fontColour, UI_Text* textCallback, UI* parent = nullptr, p2SString* string = nullptr,
					p2SString* hoverString = nullptr, p2SString* focusString = nullptr, p2SString* leftClickString = nullptr, p2SString* rightClickString = nullptr);
	UI* CreateButton(UI_Element element, int x, int y, /*UI_Button* buttonCallback,*/ UI* parent = nullptr, SDL_Rect* idle = nullptr, SDL_Rect* hover = nullptr, SDL_Rect* clicked = nullptr);

	//j1Gui* elementCallback;
	UI* elementCallback;

	void OnEventCall(UI* element, UI_Event ui_event);					//This function is called whenever an new event happens, it receives the pointer of the element that caused the event and the kind of event it is.
	void CheckFunctionality(UI_Callback callback, UI_Event ui_event);
	void PassFocus();
	void Debug_UI();
	bool ui_debug;

	//Button pointers
	UI_Image*	debug_Image;
	UI_Text*	debug_Text;
	UI_Button*	debug_Button;
	UI_Button*	escape_Button;
	//UI* debug_Button;
	UI*			focusedElement;		//Change to list item

	bool escape;

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;

	p2List<UI*> elements;		//List where all the UI elements in a scene will be stored at.
};

#endif // __j1GUI_H__