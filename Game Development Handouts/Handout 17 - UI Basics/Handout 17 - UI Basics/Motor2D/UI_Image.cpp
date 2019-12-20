#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "UI.h"
#include "UI_Image.h"

//UI_Image will always be uninteractible and will have 2 events: IDLE & CLICKED (CLICKED when the image is clicked and can be dragged).
//UI_Image can be draggable and can have a parent element.
//hitbox argument serves the purpose of both setting the UI_Image's "input collider" and locating the right sprite in the Atlas(tex).
UI_Image::UI_Image(UI_Element element, int x, int y, SDL_Rect hitbox, bool isInteractible, bool isDraggable, UI* parent) : UI(UI_Element::IMAGE, x, y, hitbox, parent) //When a UI_Image's constructor is called, a UI element is initialized as a IMAGE element.
{
	tex = App->gui->GetAtlas();						//The atlas already has the path to the atlas spritesheet. Check how to work around the const

	this->isInteractible = isInteractible;			
	this->isDraggable = isDraggable;

	//isVisible bool?
	//isWindow bool?

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

bool UI_Image::Draw()
{
	CheckInput();
	
	BlitElement(tex, GetPosition().x, GetPosition().y, &GetRect());		//GetPosition() is used as the position variable in the UI parent class will be initialized with the values of the UI_Image instance at constructor call
	
	return true;
}

void UI_Image::CheckInput()
{
	if (isDraggable)
	{
		GetMousePos();

		bool hovered = CheckMousePos();

		if (hovered == false)																		//If the mouse is not on the image.
		{
			ui_event = UI_Event::IDLE;
		}

		if (hovered == true && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			prevMousePos = GetMousePos();
		}

		if (hovered == true && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)			//If the mouse is on the image and the left mouse button is pressed.
		{
			ui_event = UI_Event::CLICKED;

			//Drag Logic
			DragElement();
			prevMousePos = GetMousePos();
		}

		App->gui->OnEventCall(this, ui_event);
	}
}