#ifndef __UI_H__
#define __UI_H__

#include "p2Log.h"
#include "p2Point.h"
#include "j1Input.h"
#include "SDL/include/SDL.h"

//class iPoint;		//If declared as a forward declaration instead of an include, 
//class SDL_Rect;		//then iPoint and SDL_Rect need to be pointers.

enum class UI_Element
{
	IMAGE,
	TEXT,
	BUTTON,
	SCROLLBAR,
	INPUTBOX
};

enum class UI_Event
{
	IDLE,
	HOVER,
	UNHOVER,
	CLICKED,
	UNCLICKED,
	FOCUSED,
	UNFOCUSED,
	TEXT_INPUT,
	TEXT_EXECUTION
};

//enum class Element_Callback
//{
//	
//};

class UI
{
public:

	UI(UI_Element element, int x, int y, SDL_Rect rect, UI* parent = nullptr);

	virtual ~UI();

	virtual bool Draw();

public:
	void SetPosition(iPoint position);	//Sets the position of a UI_Element
	iPoint	GetPosition() const;		//Gets the position of a UI_Element
	void SetRect(SDL_Rect rect);		//Sets the data members of a UI_Element's rect.
	SDL_Rect GetRect() const;			//Gets the data members of a UI_Element's rect.
	void SetHitbox(SDL_Rect hitbox);	//Sets the data members of a UI_Element's hitbox.
	SDL_Rect GetHitbox() const;			//Gets the data members of a UI_Element's hitbox.

	iPoint GetMousePos() /*const*/;		//Gets the mouse's position.
	bool CheckMousePos();				//Checks the position of the mouse.

	void BlitElement(SDL_Texture* texture, int x, int y, SDL_Rect* rect);

	bool		interactible;			//Keeps track of whether a UI Element is interactible or not.
	bool		focused;				//Keeps track of whether a UI Element has focus or not.
	bool		hadFocus;
	UI_Event	ui_event;				//Defines which events will the UI_Elements send when interacted with.
	UI_Element	element;				//Enum that defines which kind of element a UI element is.
	UI*			parent;					//Keeps track of the dependencies between UI elements.

private:
	iPoint		position;				//Position of the UI element in the world.
	iPoint		mousePos;				//Position of the mouse.
	SDL_Rect	rect;					//Rectangle that represents the UI element in the world. Used for textures.
	SDL_Rect	hitbox;					//Rectangle that represents the UI element's hitbox. Used for interactions.

	j1Module*	listener;				//Whenever an event is triggered, this calls the right module for the event.
};

#endif // !__UI_IMAGE_H__