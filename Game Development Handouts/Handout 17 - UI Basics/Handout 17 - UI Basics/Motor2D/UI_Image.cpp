#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "UI.h"
#include "UI_Image.h"


UI_Image::UI_Image(UI_Element element, int x, int y, SDL_Rect rect, bool isInteractible, bool isDraggable, UI* parent) : UI(UI_Element::IMAGE, x, y, rect, parent) //When a UI_Image's constructor is called, a UI element is initialized as a IMAGE element.
{
	currentTex = App->gui->GetAtlas();		//The atlas already has the path to the atlas spritesheet. Check how to work around the const

	this->isInteractible = isInteractible;			//A UI_Image type of UI Element will never be interactible.
	this->isDraggable = isDraggable;

	//imgCallback = elementCallback;
}

bool UI_Image::Draw()
{
	CheckInput();
	
	BlitElement(currentTex, GetPosition().x, GetPosition().y, &GetRect());		//GetPosition() is used as the position variable in the UI parent class will be initialized with the values of the UI_Image instance at constructor call
	
	return true;
}

void UI_Image::CheckInput()
{
	if (isInteractible)
	{
		GetMousePos();

		bool hovered = CheckMousePos();

		if (hovered == false)																		//If the mouse is not on the text.
		{
			ui_event = UI_Event::IDLE;
			/*currentTex = idleTex;	*/																//Blit the idle text.
		}

		if (hovered == true || focused == true)														//If the mouse is on the image.
		{
			ui_event = UI_Event::HOVER;

			//if (hoverTex != NULL)
			//{
			//	currentTex = hoverTex;																//Blit the hover image.
			//}
		}

		if (hovered == true && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)			//If the mouse is on the image and the left mouse button is pressed.
		{
			ui_event = UI_Event::CLICKED;

			//if (leftClickTex != NULL)
			//{
			//	currentTex = leftClickTex;															//Blit the left click image.
			//}
		}

		if (hovered == true && App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)		//If the mouse is on the image and the right mouse button is pressed.
		{
			ui_event = UI_Event::CLICKED;

			//if (rightClickTex != NULL)
			//{
			//	currentTex = rightClickTex;															//Blit the right click image.
			//}
		}

		App->gui->OnEventCall(this, ui_event);
	}
	else
	{
		ui_event = UI_Event::IDLE;
		//currentTex = idleTex;
	}
	
}