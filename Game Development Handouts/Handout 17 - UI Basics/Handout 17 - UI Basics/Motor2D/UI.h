#ifndef __UI_H__
#define __UI_H__

#include "p2Log.h"
#include "p2Point.h"
#include "j1Input.h"
#include "SDL/include/SDL.h"

//class iPoint;			//If declared as a forward declaration instead of an include, 
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

class UI
{
public:

	UI(UI_Element element, int x, int y, SDL_Rect rect, UI* parent = nullptr);

	virtual ~UI();

	virtual bool Draw();

	virtual void CheckInput();

public:
	void SetPosition(iPoint position);		//Sets the position of a UI_Element
	iPoint GetPosition() const;				//Gets the position of a UI_Element
	
	void SetRect(SDL_Rect rect);			//Sets the data members of a UI_Element's rect.
	SDL_Rect GetRect() const;				//Gets the data members of a UI_Element's rect.
	
	void SetHitbox(SDL_Rect hitbox);		//Sets the data members of a UI_Element's hitbox.
	SDL_Rect GetHitbox() const;				//Gets the data members of a UI_Element's hitbox.

	//iPoint GetScreenPos() const;			//Gets the position of a UI Element with the Screen as point of reference.
	//SDL_Rect GetScreenRect() const;			//Gets the data members of the Screen's rect.?

	iPoint GetLocalPos() const;				//Gets the position of a UI Element with it's Parent as point of reference.
	void SetLocalPos(iPoint localPosition);	//Sets the position of a UI Element with it's Parent as point of reference.

	SDL_Rect GetLocalRect() const;			//Get the data members of the UI Element's Parent's rect.

	iPoint GetMousePos() /*const*/;			//Gets the mouse's position.
	bool CheckMousePos();					//Checks the position of the mouse.

	iPoint GetMouseMotion() /*const*/;		//Gets the mouse's motion.

	bool IsFocused() const;					//Centralize common functionalities.

	void DragElement();

	
	void OnEventCall(UI* element, UI_Event ui_event);

	void BlitElement(SDL_Texture* texture, int x, int y, SDL_Rect* rect);

public:
	bool		isVisible;					//Keep track of whether or not a UI Element is visible or not. Can be overlapped with isInteractible.
	bool		isInteractible;				//Keeps track of whether a UI Element is interactible or not.
	bool		isDraggable;				//Keeps track of whether a UI Element is draggable or not.
	bool		hasBeenDragged;				//Keeps track of whether a UI Element has been dragged in an event check. Used to avoid clicking on the UI_Element after dragging it.
	bool		focused;					//Keeps track of whether a UI Element has focus or not.
	iPoint		prevMousePos;

	UI_Event	ui_event;					//Defines which events will the UI_Elements send when interacted with.
	UI_Element	element;					//Enum that defines which kind of element a UI element is.

	j1Module*	listener;					//Callback to j1Module, maybe need to make a virtual event detection function. Whenever an event is triggered, this calls the right module for the event.
	UI*			parent;						//Keeps track of the dependencies between UI elements.

private:
	iPoint		position;					//Position of the UI element in the world.
	iPoint		localPosition;				//Position of the UI element relative to its parent's position in the world.
	iPoint		initialPosition;			//Keeps track of the initial position of a UI Element.

	iPoint		mousePos;					//Position of the mouse.
	iPoint		mouseMotion;				//Motion of the mouse. Used to move a dragged element around.

	SDL_Rect	rect;						//Rectangle that represents the UI element in the world. Used for textures.
	SDL_Rect	hitbox;						//Rectangle that represents the UI element's hitbox. Used for interactions.
};

#endif // !__UI_IMAGE_H__