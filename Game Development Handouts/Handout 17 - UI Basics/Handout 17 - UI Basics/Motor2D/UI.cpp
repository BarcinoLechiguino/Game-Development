#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "UI.h"
#include "UI_Image.h"
#include "UI_Text.h"

UI::UI(UI_Element element, int x, int y, SDL_Rect rect, UI* parent) :
	element(element),										//The variables of the UI element are initialized at the constructor.
	position(x, y),
	rect(rect),
	parent(parent)
{
	//hitbox = {GetPosition().x, GetPosition().y, GetRect().w, GetRect().h };
	hitbox = {position.x, position.y, this->rect.w, this->rect.h };
	focused = false;
}

UI::~UI()
{

}

bool UI::Draw()
{	
	return true;
}

void UI::SetPosition(iPoint position)
{
	this->position = position;			//this-> Allows to overload the position variable name as it declares that the "position" we are setting is the one declared in UI.h.
}

iPoint UI::GetPosition() const
{
	return position;					//Returns the position of a UI element.
}

void UI::SetRect(SDL_Rect rect)
{
	this->rect = rect;					//this-> Sets the pointer of a UI_Element so only that UI element's rect is changed. Moreover it allows to overload the rect variable name.
}

SDL_Rect UI::GetRect() const
{
	return rect;						//Returns the rect of a UI element.
}

void UI::SetHitbox(SDL_Rect hitbox)
{
	this->hitbox = hitbox;				//this-> Sets the pointer of a UI_Element so only that UI element's hitbox is changed. Moreover it allows to overload the hitbox variable name.
}

SDL_Rect UI::GetHitbox() const
{
	return hitbox;						//Returns the hitbox of a UI element.
}

iPoint UI::GetMousePos() /*const*/
{
	App->input->GetMousePosition(mousePos.x, mousePos.y);

	return mousePos;
}

bool UI::CheckMousePos()
{
	return(mousePos.x > hitbox.x && mousePos.x < hitbox.x + hitbox.w 
		&& mousePos.y > hitbox.y && mousePos.y < hitbox.y + hitbox.h);
}

//void UI::OnEventCall(UI* element, UI_Event ui_event)
//{
//	if (element == App->scene->button && ui_event == UI_Event::CLICKED)
//	{
//		if (App->gui->ui_debug == false)
//		{
//			App->gui->ui_debug = true;
//		}
//		else
//		{
//			App->gui->ui_debug = false;
//		}
//	}
//}

void UI::BlitElement(SDL_Texture* texture, int x, int y, SDL_Rect* rect)
{
	App->render->Blit(texture, x, y, rect, 0.0f);		//Setting the blit's speed argument to 0 will make the UI Element remain static in relation to the camera.
}