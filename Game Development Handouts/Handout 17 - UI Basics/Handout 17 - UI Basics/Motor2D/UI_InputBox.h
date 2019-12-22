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
	UI_InputBox();
	UI_InputBox(UI_Element element, int x, int y, SDL_Rect hitbox, _TTF_Font* font, SDL_Color fontColour, SDL_Rect cursorSize, SDL_Color cursorColor, iPoint textOffset,
		bool isVisible = true, bool isInteractible = true, bool isDraggable = false, UI* parent = nullptr, p2SString* string = nullptr);

	bool Draw();

	void CheckInput();

	void DrawInputBoxElements();
	void DrawCursor();
	
	void CheckFocus();

	void UpdateInputBoxElementsPos();
	void SetInputBoxElementsVisibility();

private:
	SDL_Texture*	tex;
	UI_Image		background;
	UI_Text			text;
	UI_Image		cursor;
	
	//SDL_Rect		cursor;				//Text cursor. Blinks at cretain times.
	SDL_Color		cursorColour;		//Colour of the cursor.

	//char*			inputText;
	//p2SString		inputText;
	_TTF_Font*		font;				//Loaded font.
	int				textWidth;
	int				textHeight;

	int				letterCount;		//Amount of letters currently in the text.
	iPoint			textOffset;			//Difference in origin position of the text from the background.
};

#endif // !__UI_INPUTBOX_H__
