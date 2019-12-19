#include "j1App.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "UI_Button.h"

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
}

bool UI_Button::Draw()
{
	CheckInput();														//Calling "Update" and Draw at the same time. 
	
	BlitElement(tex, GetPosition().x, GetPosition().y, &currentRect);

	return true;
}

void UI_Button::CheckInput()
{
	GetMousePos();

	bool hovered = CheckMousePos();

	if (hovered == false)												//If the mouse is not on the button.
	{
		ui_event = UI_Event::IDLE;
		currentRect = idle;												//Button Idle sprite.
	}

	if (focused == true)
	{
		LOG("The Button Has focus");
	}

	if (hovered == true || /*focused == true*/ IsFocused() == true)		//If the mouse is on the button.
	{
		ui_event = UI_Event::HOVER;
		currentRect = hover;											//Button Hover sprite.
	}

	if (hovered == true && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)		//If the mouse is on the button and the left mouse button is pressed.
	{
		ui_event = UI_Event::CLICKED;
		currentRect = clicked;											//Button Hover sprite.
	}
	
	App->gui->OnEventCall(this, ui_event);								//This UI element's pointer and ui_event are passed as arguments to the OnEventCall() function.
	//listener->OnEventCall(this, ui_event);							//At some point set j1Gui (or UI) as the listener for this EventCall.
}