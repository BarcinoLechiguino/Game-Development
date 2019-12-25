#include "j1App.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "UI_Image.h"
#include "UI_Scrollbar.h"

UI_Scrollbar::UI_Scrollbar(UI_Element element, int x, int y, SDL_Rect hitbox, SDL_Rect thumbSize, iPoint thumbOffset, SDL_Rect dragArea, float dragFactor,
					bool isVisible, bool isInteractible, bool isDraggable, UI* parent) : UI(element, x, y, hitbox)
{
	tex = App->gui->GetAtlas();

	if (isInteractible)																//If the Input Box element is interactible.
	{
		listener = App->gui;														//This button's listener is set to the App->gui module (For OnCallEvent()).
	}

	if (parent != NULL)																//If a parent is passed as argument.
	{
		int localPosX = x - parent->GetScreenPos().x;								//Gets the local position of the Button element in the X Axis.
		int localPosY = y - parent->GetScreenPos().y;								//Gets the local position of the Button element in the Y Axis.

		iPoint localPos = { localPosX,localPosY };									//Buffer iPoint to pass it as argument to SetLocalPos().

		SetLocalPos(localPos);														//Sets the local poisition of this Button element to the given localPos.
	}

	// ----------------------------- SCROLLBAR VARIABLES ---------------------------------
	// --- Scrollbar Flags
	this->isVisible = isVisible;													//Sets the isVisible flag of the Scrollbar to the one passed as argument.
	this->isInteractible = isInteractible;											//Sets the isInteractible flag of the Scrollbar to the one passed as argument.
	this->isDraggable = isDraggable;												//Sets the isDraggable flag of the Scrollbar to the one passed as argument.
	prevMousePos = iPoint(0, 0);													//Initializes prevMousePos for this UI Element. Safety measure to avoid weird dragging behaviour.
	initialPosition = GetScreenPos();												//Records the initial position where the input box is at app execution start.

	// --- Scrollbar Elements
	bar = UI_Image(UI_Element::IMAGE, x, y, hitbox, true, false, false, this);
	thumb = UI_Image(UI_Element::IMAGE, x + thumbOffset.x, y + thumbOffset.y, thumbSize, true, true, true, this);

	/*bar2 = (UI_Image*)App->gui->CreateImage(UI_Element::IMAGE, x, y, hitbox, true, false, false, this);
	thumb2 = (UI_Image*)App->gui->CreateImage(UI_Element::IMAGE, x + thumbOffset.x, y + thumbOffset.y, thumbSize, true, true, true, this);*/

	// --- Other Scrollbar Variables
	this->dragArea = dragArea;
	this->dragFactor = dragFactor;
	// -----------------------------------------------------------------------------------
}

bool UI_Scrollbar::Draw()
{
	CheckInput();

	DrawScrollbarElements();

	//BlitElement()

	return true;
}

void UI_Scrollbar::CheckInput()
{
	if (isVisible)																				//If the image element is visible
	{
		GetMousePos();																			//Gets the mouse's position on the screen.

		if (!IsHovered())																		//If the mouse is not on the image.
		{
			ui_event = UI_Event::IDLE;
		}

		if (isDraggable)																		//If the image element is draggable.
		{
			if (IsHovered() && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)		//If the mouse is on the image and the left mouse button is pressed.
			{
				if (IsForemostElement())
				{
					prevMousePos = GetMousePos();												//Sets the initial position where the mouse was before starting to drag the element.
					initialPosition = GetScreenPos();											//Sets initialPosition with the current position at mouse KEY_DOWN.
					isDragTarget = true;														//Sets the element as the drag target.
				}
			}

			if (IsHovered() && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)	//If the mouse is on the image and the left mouse button is continuously pressed.
			{
				if (IsForemostElement());														//If it is the first element under the mouse (in inverse order of draw)
				{
					ui_event = UI_Event::CLICKED;

					if (ElementCanBeDragged())													//If the UI Image element is draggable and is the foremost element under the mouse.
					{
						DragElement();															//The element is dragged around.

						CheckElementChilds();													//Checks if this image element has any childs and updates them in case the image element (parent) has had any change in position.

						prevMousePos = GetMousePos();											//prevMousePos is set with the new position where the mouse is after dragging for a frame.
					}
				}
			}

			if (IsHovered() && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
			{
				if (/*IsForemostElement() &&*/ isDragTarget)
				{
					isDragTarget = false;
				}
			}

			if (isInteractible)																	//If the image element is interactible.
			{
				listener->OnEventCall(this, ui_event);											//The listener call the OnEventCall() method passing this UI_Image and it's event as arguments.
			}
		}
	}
}

void UI_Scrollbar::DrawScrollbarElements()
{
	bar.Draw();
	thumb.Draw();
}

void UI_Scrollbar::LinkScroll()
{

}