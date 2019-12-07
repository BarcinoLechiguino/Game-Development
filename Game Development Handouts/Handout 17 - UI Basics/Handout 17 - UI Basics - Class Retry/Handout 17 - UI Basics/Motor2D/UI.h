#ifndef __j1UI_H__
#define __j1UI_H__

#include "p2Point.h"
#include "SDL/include/SDL.h"

enum class UI_ELEMENT
{
	NONE,
	IMAGE,
	TEXT,
	TOGGLE,
	SCROLLBAR,
	CHECKPOINT,
	UNKNOWN
};

enum class UI_EVENT		//Events are generated when a button is pressed... Whatever that event means is decided by the method called at button press.
{
	HOVER,				//
	UNHOVER,
	CLICKED,
	RELEASED,
	INPUT_CHANGED,		//Detects that the input box text has been changed and does sth accordingly
	INPUT_SUBMIT,		//Detects that the input box text has been submitted (ENTER, Button click...)
	GAIN_FOCUS,
	LOSE_FOCUS
};

class UI
{
public:
	UI(UI_ELEMENT element, int x, int y, SDL_Rect rect);
	
	virtual ~UI();

	virtual bool Update();

	virtual bool Draw();

private:
	UI_ELEMENT	element;
	SDL_Rect	rectangle;
	iPoint		position;
};

#endif // __j1UI_H__