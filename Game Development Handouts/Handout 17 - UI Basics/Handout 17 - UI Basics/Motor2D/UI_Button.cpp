#include "j1App.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "UI_Button.h"

UI_Button::UI_Button(UI_Element element, int x, int y, SDL_Rect* idle, SDL_Rect* hover, SDL_Rect* clicked, UI* parent) : UI(element, x, y, *idle, parent)
{
	tex = App->gui->GetAtlas();

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

	interactible = true;

	//focused = false;
}

bool UI_Button::Draw()
{
	GetMousePos();

	bool hovered = CheckMousePos();

	if (hovered == false)										//If the mouse is not on the button.
	{
		ui_event = UI_Event::IDLE;
		currentRect = idle;										//Button Idle sprite.
	}

	if (focused == true)
	{
		LOG("The Button Has focus");
	}

	if (hovered == true || focused == true)						//If the mouse is on the button.
	{
		ui_event = UI_Event::HOVER;
		currentRect = hover;									//Button Hover sprite.
	}

	if (hovered == true && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) //If the mouse is on the button and the left mouse button is pressed.
	{
		ui_event = UI_Event::CLICKED;
		currentRect = clicked;									//Button Hover sprite.
	}

	BlitElement(tex, GetPosition().x, GetPosition().y, &currentRect);

	return true;
}