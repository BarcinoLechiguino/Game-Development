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
		bool isVisible = true, bool isInteractible = true, bool isDraggable = false, UI* parent = nullptr, p2SString* defaultString = nullptr);

	bool Draw();

	void CheckInput();

	void DrawInputBoxElements();
	void DrawCursor();
	
	void CheckInputBoxState();
	void CheckFocus();

	void UpdateInputBoxElementsPos();
	void SetInputBoxElementsVisibility();

	void CheckCursorInputs();
	void RefreshInputText();

private:
	SDL_Texture*	tex;
	UI_Image		background;					//Background element of the Input Box.
	UI_Text			text;						//Text element of the Input Box.
	UI_Image		cursor;						//Cursor element of the Input Box.
	
	SDL_Color		cursorColour;				//Colour of the cursor.

	_TTF_Font*		font;						//Loaded font.
	int				textWidth;					//Current width of the text.
	int				textHeight;					//Current height of the text.
	int				prevTextWidth;
	int				prevTextHeight;

	int				prevLength;					//Amount of letters currently in the text.
	int				cursorIndex;				//Each number of the cursorIndex corresponds with a letter. First letter: cursorIndex = 1...
	bool			freeCursor;					//Keeps track whether the cursor is at the text's last letter or if it is moving around.
	int				cursorPositions[500];		//Array that will keep track of all the positions that the cursor has been in. Each postion correspond with a cursor index.
	iPoint			textOffset;					//Difference in origin position between the background's and the text's.
};

#endif // !__UI_INPUTBOX_H__
