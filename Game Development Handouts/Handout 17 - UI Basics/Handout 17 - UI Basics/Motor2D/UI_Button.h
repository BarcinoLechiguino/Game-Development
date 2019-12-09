#ifndef __UI_BUTTON_H__
#define __UI_BUTTON_H__

#include "UI.h"

class UI_Button : public UI
{
public:

	//elementCallback is the pointer to the button itself (loginButton, escapeButton...) and it is needed to have multiple different pointers for each kind of button. 
	//Callback is an enum used to define the functionality of a button.
	UI_Button(UI_Element element, int x, int y, /* UI_Button* elementCallback,*/ UI* parent = nullptr, SDL_Rect* idle = nullptr, SDL_Rect* hover = nullptr, SDL_Rect* clicked = nullptr);

	bool Draw();

	void CheckInput();

private:
	SDL_Texture*	tex;
	SDL_Rect		idle;			//Sprite for the button's idle event.
	SDL_Rect		hover;			//Sprite for the button's hover event.
	SDL_Rect		clicked;		//Sprite for the button's clicked event.
	SDL_Rect		currentRect;	//Rect that will hold the correct sprite for each state.

	//UI_Button*		buttonCallback;		//Pointer of the button, if we want to assign a function to a specific button, the pointer needs to be compared
};

#endif // !__UI_BUTTON_H__