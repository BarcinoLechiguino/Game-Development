#ifndef __UI_TEXT_H__
#define __UI_TEXT_H__

#include "p2SString.h"
#include "j1Fonts.h"
#include "UI.h"

//class j1Fonts;

class UI_Text : public UI
{
public:

	UI_Text(UI_Element type, int x, int y, SDL_Rect hitbox, p2SString* string);

	bool Draw();

private:
	p2SString* string;	//String of the UI Text element
	_TTF_Font* font;
	SDL_Texture* tex;
};

#endif // !__UI_TEXT_H__