#include "j1App.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "UI_Image.h"
#include "UI_Scrollbar.h"

UI_Scrollbar::UI_Scrollbar(UI_Element element, int x, int y, SDL_Rect hitbox, SDL_Rect thumbSize, iPoint thumbOffset, SDL_Rect dragArea, float dragFactor, bool dragXAxis, bool dragYAxis,
				bool invertedScrolling, bool isVisible, bool isInteractible, bool isDraggable, UI* parent, SDL_Rect* scrollMask, iPoint maskOffset) : UI(element, x, y, hitbox, parent)
{
	tex = App->gui->GetAtlas();

	if (isInteractible)																//If the Scrollbar element is interactible.
	{
		listener = App->gui;														//This scrollbar's listener is set to the App->gui module (For OnCallEvent()).
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
		// --- Setting this element's flags to the ones passed as argument.
	this->isVisible = isVisible;												//Sets the isVisible flag to the one passed as argument.
	this->isInteractible = isInteractible;										//Sets the isInteractible flag to the one passed as argument. 
	this->isDraggable = isDraggable;											//Sets the isDraggable flag to the one passed as argument.
	this->dragXAxis = dragXAxis;												//Sets the dragXaxis flag to the one passed as argument.
	this->dragYAxis = dragYAxis;												//Sets the dragYaxis flag to the one passed as argument.
	prevMousePos = iPoint(0, 0);												//Initializes prevMousePos for this UI Element. Safety measure to avoid weird dragging behaviour.
	initialPosition = GetScreenPos();											//Records the initial position where the element is at at app execution start.											//Records the initial position where the input box is at app execution start.

	// --- Scrollbar Elements
	/*bar = UI_Image(UI_Element::IMAGE, x, y, hitbox, true, false, false, this);
	thumb = UI_Image(UI_Element::IMAGE, x + thumbOffset.x, y + thumbOffset.y, thumbSize, true, true, true, this);*/

	bar = (UI_Image*)App->gui->CreateImage(UI_Element::IMAGE, x, y, hitbox, isVisible, false, false, this);
	thumb = (UI_Image*)App->gui->CreateImage(UI_Element::IMAGE, x + thumbOffset.x, y + thumbOffset.y, thumbSize, isVisible, true, true, this);

	if (scrollMask != nullptr)
	{
		this->scrollMask = (UI_Image*)App->gui->CreateImage(UI_Element::EMPTY, x + maskOffset.x, y + maskOffset.y, *scrollMask, false, false, false, this);
		//this->scrollMask.
		//this->scrollMask = UI_Image(UI_Element::IMAGE, x + maskOffset.x, y + maskOffset.y, *scrollMask, false, false, false, this);
	}

	// --- Other Scrollbar Variables
	this->dragArea				= {x, y, dragArea.w, dragArea.h};
	this->dragFactor			= dragFactor;
	this->dragXAxis				= dragXAxis;
	this->dragYAxis				= dragYAxis;
	this->invertedScrolling		= invertedScrolling;
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
		
		if (IsFocused())
		{
			DragThumbWithMousewheel();
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
					App->gui->focusedElement = this;											//Set the focus on the Input Box element when it is clicked.
				}
			}

			if (!IsHovered() && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)		//Unfocus when the mouse clicks outside the input box.
			{
				if (App->gui->focusedElement == this)
				{
					App->gui->focusedElement = nullptr;
				}
			}

			if ((IsHovered() || isDragTarget) && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)	//If the mouse is on the image and the left mouse button is continuously pressed.
			{
				if (IsForemostElement() || isDragTarget);														//If it is the first element under the mouse (in inverse order of draw)
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
					initialPosition = GetScreenPos();
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
	//bar.Draw();
	//thumb.Draw();
	//scrollMask->isVisible = false;

	UpdateLinkedElements();
}

void UI_Scrollbar::LinkScroll(UI* element)
{
	linkedElements.add(element);
}

void UI_Scrollbar::UpdateLinkedElements()
{
	for (p2List_item<UI*>* element = linkedElements.start; element != NULL; element = element->next)
	{
		UI* elem = element->data;

		if (thumb->GetScreenPos() != thumb->initialPosition)
		{
			if (dragXAxis)
			{
				dragFactor = GetDragFactor(elem);
				float x_PositionOffset = (thumb->GetScreenPos().x - thumb->initialPosition.x) * dragFactor;

				if (invertedScrolling)
				{
					dragDisplacement = iPoint(elem->GetScreenPos().x - x_PositionOffset, elem->GetLocalPos().y);
				}
				else
				{
					dragDisplacement = iPoint(elem->GetScreenPos().x + x_PositionOffset, elem->GetLocalPos().y);
				}
			}
			if (dragYAxis)
			{
				dragFactor = GetDragFactor(elem);
				float y_PositionOffset = (thumb->GetScreenPos().y - thumb->initialPosition.y) * dragFactor;

				if (invertedScrolling)
				{
					dragDisplacement = iPoint(elem->GetScreenPos().x, elem->GetLocalPos().y - y_PositionOffset);
				}
				else
				{
					dragDisplacement = iPoint(elem->GetScreenPos().x, elem->GetLocalPos().y + y_PositionOffset);
				}
			}
			
			elem->SetScreenPos(dragDisplacement);
			elem->SetHitbox({ elem->GetScreenPos().x
							, elem->GetScreenPos().y
							, elem->GetHitbox().w
							, elem->GetHitbox().h });
		}
	}
}

float UI_Scrollbar::GetDragFactor(UI* element)
{
	if (dragXAxis == dragYAxis)
	{
		return 0.0f;
	}
	
	if (dragXAxis)
	{
		float elemWidth = element->GetHitbox().w;
		float dragAreaWidth = dragArea.w;
		dragFactor = elemWidth / dragAreaWidth;
	}
	if (dragYAxis)
	{
		float elemHeight = element->GetHitbox().h;
		float dragAreaHeight = dragArea.h;
		dragFactor = (elemHeight / dragAreaHeight);
	}

	return dragFactor;
}

bool UI_Scrollbar::GetDragXAxis()
{
	return dragXAxis;
}

bool UI_Scrollbar::GetDragYAxis()
{
	return dragYAxis;
}

void UI_Scrollbar::DragThumbWithMousewheel()
{
	App->input->GetMousewheelScrolling(mouseWheelScroll.x, mouseWheelScroll.y);

	if (dragXAxis)
	{
		if (invertedScrolling)
		{
			thumb->SetScreenPos(iPoint(thumb->GetScreenPos().x - mouseWheelScroll.x, thumb->GetScreenPos().y));
			thumb->SetHitbox({ thumb->GetScreenPos().x, thumb->GetScreenPos().y, thumb->GetHitbox().w, thumb->GetHitbox().h });
		}
		else
		{
			thumb->SetScreenPos(iPoint(thumb->GetScreenPos().x + mouseWheelScroll.x, thumb->GetScreenPos().y));
			thumb->SetHitbox({ thumb->GetScreenPos().x, thumb->GetScreenPos().y, thumb->GetHitbox().w, thumb->GetHitbox().h });
		}
	}

	if (dragYAxis)
	{
		if (invertedScrolling)
		{
			thumb->SetScreenPos(iPoint(thumb->GetScreenPos().x, thumb->GetScreenPos().y - mouseWheelScroll.y));
			thumb->SetHitbox({ thumb->GetScreenPos().x, thumb->GetScreenPos().y, thumb->GetHitbox().w, thumb->GetHitbox().h });
		}
		else
		{
			thumb->SetScreenPos(iPoint(thumb->GetScreenPos().x, thumb->GetScreenPos().y + mouseWheelScroll.y));
			thumb->SetHitbox({ thumb->GetScreenPos().x, thumb->GetScreenPos().y, thumb->GetHitbox().w, thumb->GetHitbox().h });
		}
	}
}

bool UI_Scrollbar::ThumbIsWithinScrollbarBounds()
{
	return true;
}

bool UI_Scrollbar::ThumbAtUpperBound()
{
	return true;
}

bool UI_Scrollbar::ThumbAtLowerBound()
{
	return true;
}