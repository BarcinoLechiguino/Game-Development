#ifndef __UI_INPUTBOX_H__
#define __UI_INPUTBOX_H__

#include "UI.h"
//#include "UI_Image.h"
//#include "UI_Text.h"

class UI_Image;
class UI_Text;

class UI_InputBox : public UI
{
public:
	UI_InputBox(UI_Element element, int x, int y, SDL_Rect hitbox, _TTF_Font* font, SDL_Color fontColour, bool isVisible = true, bool isInteractible = true, bool isDraggable = false,
		UI* parent = nullptr, p2SString* string = nullptr);

	bool Draw();

	void CheckInput();

private:
	SDL_Texture*	tex;
	UI_Image*		background;
	UI_Text*		text;
	SDL_Rect		cursor;
	int				letterCount;

};

#endif // !__UI_INPUTBOX_H__
