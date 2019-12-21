#include "j1App.h"
#include "j1Input.h"
#include "j1Gui.h"
//#include "UI.h"
#include "UI_Image.h"
#include "UI_Text.h"
#include "UI_InputBox.h"

UI_InputBox::UI_InputBox(UI_Element element, int x, int y, SDL_Rect hitbox, _TTF_Font* font, SDL_Color fontColour, bool isVisible, bool isInteractible, bool isDraggable, UI* parent,
	p2SString* string) : UI(element, x, y, hitbox, parent)
{
	tex = App->gui->GetAtlas();

	if (background != nullptr)
	{
		this->background = background;
	}

	if (text != nullptr)
	{
		this->text = text;
	}

	this->isVisible = isVisible;
	this->isInteractible = isInteractible;
	this->isDraggable = isDraggable;
	prevMousePos = iPoint(0, 0);

	if (this->isInteractible)												//If the Input Box element is interactible.
	{
		listener = App->gui;												//This button's listener is set to the App->gui module (For OnCallEvent()).
	}

	if (parent != NULL)														//If a parent is passed as argument.
	{
		int localPosX = x - this->parent->GetScreenPos().x;					//Gets the local position of the Button element in the X Axis.
		int localPosY = y - this->parent->GetScreenPos().y;					//Gets the local position of the Button element in the Y Axis.

		iPoint localPos = { localPosX,localPosY };							//Buffer iPoint to pass it as argument to SetLocalPos().

		SetLocalPos(localPos);												//Sets the local poisition of this Button element to the given localPos.
	}

	this->background = (UI_Image*)App->gui->CreateImage(UI_Element::IMAGE, x, y, hitbox, true, false, false, this);
	this->text = (UI_Text*)App->gui->CreateText(UI_Element::TEXT, x, y, hitbox, font, fontColour, true, false, false, this, string);
}

bool UI_InputBox::Draw()
{
	CheckInput();

	background->Draw();
	text->Draw();

	//BlitElement(tex, GetScreenPos().x, GetScreenPos().y, &GetScreenRect());

	return true;
}

void UI_InputBox::CheckInput()
{

}