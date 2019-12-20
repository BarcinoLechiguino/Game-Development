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
	//listener = App->gui;
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

//iPoint UI::GetScreenPos() const
//{
//	return position;
//}
//
//SDL_Rect UI::GetScreenRect() const
//{
//	return rect;
//}

iPoint UI::GetLocalPos() const
{
	return localPosition;
}

void UI::SetLocalPos(iPoint localPosition)
{
	/*int localPosX = this->parent->GetPosition().x - this->GetPosition().x;			//Useful to make this function  argumentless.
	int localPosY = this->parent->GetPosition().y - this->GetPosition().y;*/

	int localPosX = this->parent->position.x - localPosition.x;
	int localPosY = this->parent->position.y - localPosition.y;

	iPoint localPos = { localPosX, localPosY };
	
	this->localPosition = localPos;
}

SDL_Rect UI::GetLocalRect() const
{
	if (parent != NULL)
	{
		return parent->rect;
	}
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

iPoint UI::GetMouseMotion() /*const*/
{
	App->input->GetMouseMotion(mouseMotion.x, mouseMotion.y);

	return mouseMotion;
}

bool UI::IsFocused() const
{
	return App->gui->focusedElement == this;
}

void UI::DragElement()
{
	// --- Updating the UI Element's position when it is being dragged.
	iPoint currentPos = position;
	iPoint fastMouseMotion = { GetMouseMotion().x * 2, GetMouseMotion().y * 2 };
	iPoint draggingPos = position + GetMouseMotion();

	if (prevMousePos != GetMousePos())
	{
		currentPos += GetMousePos() - prevMousePos;			//Check this, mouse is not at a fixed point, the UI Element moves too slow.

		this->SetPosition(/*draggingPos*/ currentPos);
	}

	// --- Updating the UI Element's hitbox's rect when it is being dragged.
	SDL_Rect newPosRect = { position.x, position.y, rect.w, rect.h };

	this->SetHitbox(newPosRect);
}

void UI::CheckInput()
{
	return;
}

void UI::BlitElement(SDL_Texture* texture, int x, int y, SDL_Rect* rect)
{
	App->render->Blit(texture, x, y, rect, 0.0f);		//Setting the blit's speed argument to 0 will make the UI Element remain static in relation to the camera.
}