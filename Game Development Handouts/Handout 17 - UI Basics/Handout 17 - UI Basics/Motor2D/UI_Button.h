#ifndef __UI_BUTTON_H__
#define __UI_BUTTON_H__

#include "UI.h"

class UI_Button : public UI
{
public:

	UI_Button(UI_Element element, int x, int y, SDL_Rect* idle = nullptr, SDL_Rect* hover = nullptr, SDL_Rect* clicked = nullptr, UI* parent = nullptr);

	bool Draw();

private:
	SDL_Texture*	tex;
	SDL_Rect		idle;			//Sprite for the button's idle event.
	SDL_Rect		hover;			//Sprite for the button's hover event.
	SDL_Rect		clicked;		//Sprite for the button's clicked event.
	SDL_Rect		currentRect;	//Rect that will hold the correct sprite for each state.
};

#endif // !__UI_BUTTON_H__