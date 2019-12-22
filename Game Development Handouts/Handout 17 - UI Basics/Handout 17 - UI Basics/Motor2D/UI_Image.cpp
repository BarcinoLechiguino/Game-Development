#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "UI.h"
#include "UI_Image.h"

//UI_Image will always be uninteractible and will have 2 events: IDLE & CLICKED (CLICKED when the image is clicked and can be dragged).
//UI_Image can be draggable and can have a parent element.
//hitbox argument serves the purpose of both setting the UI_Image's "input collider" and locating the right sprite in the Atlas(tex).
UI_Image::UI_Image(UI_Element element, int x, int y, SDL_Rect hitbox, bool isVisible, bool isInteractible, bool isDraggable, UI* parent) : UI(UI_Element::IMAGE, x, y, hitbox, parent) //When a UI_Image's constructor is called, a UI element is initialized as a IMAGE element.
{
	tex = App->gui->GetAtlas();												//The atlas already has the path to the atlas spritesheet. Check how to work around the const

	// --- Setting this element's flags to the ones passed as argument.
	this->isVisible = isVisible;
	this->isInteractible = isInteractible;			
	this->isDraggable = isDraggable;
	prevMousePos = iPoint(0, 0);

	if (this->isInteractible)													//If the Image element is interactible.
	{
		listener = App->gui;													//This Image's listener is set to the App->gui module (For OnCallEvent()).
	}

	if (parent != NULL)															//If a parent is passed as argument.
	{
		int localPosX = x - parent->GetScreenPos().x;							//Gets the local position of the Image element in the X Axis.
		int localPosY = y - parent->GetScreenPos().y;							//Gets the local position of the Image element in the Y Axis.
		
		iPoint localPos = { localPosX, localPosY };								//Buffer iPoint to pass it as argument to SetLocalPos().

		SetLocalPos(localPos);													//Sets the local poisition of this Image Element to the given localPos.
	}
}

UI_Image::UI_Image() : UI()	//Default Constructor
{}

bool UI_Image::Draw()
{
	CheckInput();
	
	BlitElement(tex, GetScreenPos().x, GetScreenPos().y, &GetScreenRect());		//GetPosition() is used as the position variable in the UI parent class will be initialized with the values of the UI_Image instance at constructor call
	
	return true;
}

// --- This Method checks for any inputs that the UI_Image element might have received and "returns" an event.
void UI_Image::CheckInput()
{
	if (isVisible)																					//If the image element is visible
	{
		GetMousePos();																				//Gets the mouse's position on the screen.

		bool hovered = CheckMousePos();																//Sets a buffer with the bool returned from CheckMousePos(). Done for readability.

		if (!hovered)																				//If the mouse is not on the image.
		{
			ui_event = UI_Event::IDLE;
		}

		if (isDraggable)																			//If the image element is draggable.
		{
			if (hovered && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)				//If the mouse is on the image and the left mouse button is pressed.
			{
				if (IsForemostElement())
				{
					prevMousePos = GetMousePos();													//Sets the initial position where the mouse was before starting to drag the element.
					initialPosition = GetScreenPos();												//Sets initialPosition with the current position at mouse KEY_DOWN.
					isDragTarget = true;															//Sets the element as the drag target.
				}
			}

			if (hovered && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)			//If the mouse is on the image and the left mouse button is continuously pressed.
			{
				if (IsForemostElement());															//If it is the first element under the mouse (in inverse order of draw)
				{
					ui_event = UI_Event::CLICKED;
					
					if (ElementCanBeDragged())														//If the UI Image element is draggable and is the foremost element under the mouse.
					{
						DragElement();																//The element is dragged around.

						CheckElementChilds();														//Checks if this image element has any childs and updates them in case the image element (parent) has had any change in position.

						prevMousePos = GetMousePos();												//prevMousePos is set with the new position where the mouse is after dragging for a frame.
					}
				}
			}

			if (hovered && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
			{
				if (/*IsForemostElement() &&*/ isDragTarget)
				{
					isDragTarget = false;
				}
			}

			if (isInteractible)																		//If the image element is interactible.
			{
				listener->OnEventCall(this, ui_event);												//The listener call the OnEventCall() method passing this UI_Image and it's event as arguments.
			}
		}
	}
}