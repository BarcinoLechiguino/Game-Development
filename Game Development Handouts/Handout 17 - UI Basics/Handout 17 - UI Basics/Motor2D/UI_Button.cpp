#include "j1App.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "UI_Button.h"

//UI_Button can be interactible (will almost always be) and draggable. Can potentially receive all events.
//This element can receive up to 3 rects containing the coordinates of the sprites for each event (IDLE, HOVER & CLICKED).
UI_Button::UI_Button(UI_Element element, int x, int y, bool isInteractible, bool isDraggable, UI* parent,
				SDL_Rect* idle, SDL_Rect* hover, SDL_Rect* clicked) : UI(element, x, y, *idle, parent)
{
	tex = App->gui->GetAtlas();

	//If the SDL_Rect pointers are not null, then set the button rects to their data memebers.
	if (idle != NULL)
	{
		this->idle = *idle;
	}

	if (hover != NULL)
	{
		this->hover = *hover;
	}

	if (clicked != NULL)
	{
		this->clicked = *clicked;
	}

	this->isInteractible = isInteractible;
	this->isDraggable = isDraggable;
	hasBeenDragged = false;

	if (this->isInteractible)
	{
		listener = App->gui;
	}

	if (parent != NULL)
	{
		//int localPosX = parent->GetPosition().x - this->GetPosition().x;
		//int localPosY = parent->GetPosition().y - this->GetPosition().y;
		//
		////iPoint localPos = { localPosX, localPosY };

		iPoint localPos = { x, y };

		SetLocalPos(localPos);
	}
}

bool UI_Button::Draw()
{
	CheckInput();														//Calling "Update" and Draw at the same time. 

	BlitElement(tex, GetPosition().x, GetPosition().y, &currentRect);

	return true;
}

void UI_Button::CheckInput()
{
	hasBeenDragged = false;
	
	GetMousePos();
	
	bool hovered = CheckMousePos();

	if (hovered == false)												//If the mouse is not on the button.
	{
		ui_event = UI_Event::IDLE;
		currentRect = idle;												//Button Idle sprite.
	}

	if (hovered == true || /*focused == true*/ IsFocused() == true)		//If the mouse is on the button.
	{
		ui_event = UI_Event::HOVER;
		currentRect = hover;											//Button Hover sprite.
	}

	if (hovered == true && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		prevMousePos = GetMousePos();
	}

	if (hovered == true && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)		//If the mouse is on the button and the left mouse button is pressed continuously.
	{
		ui_event = UI_Event::CLICKED;
		currentRect = clicked;											//Button Clicked sprite is maintained.

		//prevMousePos = GetMousePos();
		
		if (isDraggable)
		{
			DragElement();
			hasBeenDragged = true;

			prevMousePos = GetMousePos();
		}
	}
	
	if (hovered == true && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)		//If the mouse is on the button and the left mouse button is released.
	{
		if (!isDraggable)
		{
			ui_event = UI_Event::UNCLICKED;
		}
		else
		{
			if (!hasBeenDragged)
			{
				ui_event = UI_Event::UNCLICKED;
			}
		}

		//currentRect = clicked;											//Button Hover sprite.
	}
	
	//App->gui->OnEventCall(this, ui_event);						//This UI element's pointer and ui_event are passed as arguments to the OnEventCall() function.
	listener->OnEventCall(this, ui_event);							//At some point set j1Gui (or UI) as the listener for this EventCall.
}