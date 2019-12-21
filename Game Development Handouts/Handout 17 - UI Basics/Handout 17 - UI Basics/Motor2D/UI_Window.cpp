#include "j1App.h"
#include "j1Gui.h"
#include "UI.h"
#include "UI_Window.h"

UI_Window::UI_Window(UI_Element element, int x, int y, SDL_Rect hitbox, bool isVisible, bool isInteractible, bool isDraggable, UI* parent) : UI(element, x, y, hitbox, parent) 
{
	tex = App->gui->GetAtlas();

	this->isInteractible = isInteractible;			//A UI_Image type of UI Element will never be interactible.
	this->isDraggable = isDraggable;
}

bool UI_Window::Draw()
{
	CheckInput();

	BlitElement(tex, GetScreenPos().x, GetScreenPos().y, &GetScreenRect());		//GetPosition() is used as the position variable in the UI parent class will be initialized with the values of the UI_Image instance at constructor call
	
	return true;
}

void UI_Window::CheckInput()
{
	if (isDraggable)
	{
		GetMousePos();

		bool hovered = CheckMousePos();

		if (hovered == false)																		//If the mouse is not on the image.
		{
			ui_event = UI_Event::IDLE;
		}

		if (hovered == true && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)			//If the mouse is on the image and the left mouse button is pressed.
		{
			ui_event = UI_Event::CLICKED;

			//Drag Logic
			DragElement();
			UpdateChilds();
		}

		App->gui->OnEventCall(this, ui_event);
	}
}

void UI_Window::UpdateChilds()
{

}