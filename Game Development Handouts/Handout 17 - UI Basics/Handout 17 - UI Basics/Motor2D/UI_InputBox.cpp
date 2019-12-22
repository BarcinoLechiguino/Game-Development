#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Gui.h"
//#include "UI.h"
#include "UI_Image.h"
#include "UI_Text.h"
#include "UI_InputBox.h"

UI_InputBox::UI_InputBox(UI_Element element, int x, int y, SDL_Rect hitbox, _TTF_Font* font, SDL_Color fontColour, SDL_Rect cursorSize, SDL_Color cursorColor, iPoint textOffset,
		bool isVisible, bool isInteractible, bool isDraggable, UI* parent, p2SString* string) : UI(element, x, y, hitbox, parent)
{
	tex = App->gui->GetAtlas();

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

	this->font = font;
	this->textOffset = textOffset;
	this->textWidth = 0;
	this->textHeight = 0;


	/*int width;
	int height;

	App->font->CalcSize(string->GetString(), width, height, font);

	LOG("Height and width of the text (%d, %d)", width, height);*/


	background = UI_Image(UI_Element::IMAGE, x, y, hitbox, true, false, false, this);
	text = UI_Text(UI_Element::TEXT, x + textOffset.x, y + textOffset.y, hitbox, font, fontColour, true, false, false, this, string);
	cursor = UI_Image(UI_Element::IMAGE, x + textOffset.x, y + textOffset.y, cursorSize, false, false, false, this);

	//this->SetHitbox({ x + this->textOffset.x, y + this->textOffset.y, hitbox.w, hitbox.h});
	//text->SetHitbox({ x + this->textOffset.x, y + this->textOffset.y, textHitbox.w, textHitbox.h});
}

UI_InputBox::UI_InputBox() : UI()
{}

bool UI_InputBox::Draw()
{
	CheckInput();

	DrawInputBoxElements();

	//BlitElement(tex, GetScreenPos().x, GetScreenPos().y, &GetScreenRect());

	return true;
}

void UI_InputBox::CheckInput()
{
	//SetBgAndTextVisibility();		//Not needed because if the Input Box is not visible, its Draw()  will not be called. Which means that bg Draw() and text Draw() will not be called either.

	UpdateInputBoxElementsPos();

	if (isVisible)
	{
		GetMousePos();																				//Gets the mouse's position on the screen.

		bool hovered = CheckMousePos();																//Sets a buffer with the bool returned from CheckMousePos(). Done for readability.

		// --- IDLE EVENT
		if (!hovered)																				//If the mouse is not on the text.
		{
			ui_event = UI_Event::IDLE;
		}

		if (isInteractible)																			//If the Text element is interactible.
		{
			// --- HOVER EVENT
			if ((hovered && IsForemostElement()) /*|| IsFocused()*/)									//If the mouse is on the text.
			{
				ui_event = UI_Event::HOVER;
			}

			CheckFocus();

			// --- CLICKED EVENT (Left Click)
			if (hovered && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)				//If the mouse is on the text and the left mouse button is pressed.
			{
				prevMousePos = GetMousePos();														//Sets the initial position where the mouse was before starting to drag the element.
				initialPosition = GetScreenPos();													//Sets initialPosition with the current position at mouse KEY_DOWN.
				App->gui->focusedElement = this;													//Set the focus on the Input Box element when it is clicked.
			}

			if (!hovered && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)			//Unfocus when the mouse clicks outside the input box.
			{
				if (App->gui->focusedElement == this)
				{
					App->gui->focusedElement = nullptr;
				}
			}

			if (hovered && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)			//If the mouse is on the text and the left mouse button is being pressed.
			{
				if (IsForemostElement() /*|| IsFocused()*/)											//If the UI Text element is the foremost element under the mouse. 
				{
					ui_event = UI_Event::CLICKED;

					//text.DeleteCurrentStringTex();
					//App->font->CalcSize()

					if (ElementCanBeDragged())														//If the UI Text element is draggable and is the foremost element under the mouse. 
					{
						DragElement();																//The Text element is dragged.

						CheckElementChilds();														//Checks if this Text element has any childs and updates them in consequence.

						prevMousePos = GetMousePos();												//Updates prevMousePos so it can be dragged again next frame.
					}
				}
			}

			// --- UNCLICKED EVENT (Left Click)
			if (hovered == true && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)		//If the mouse is on the text and the left mouse button is released.
			{
				if (IsForemostElement() && ElementRemainedInPlace())								//If the UI Text element is the foremost element under the mouse and has not been dragged. 
				{
					ui_event = UI_Event::UNCLICKED;
				}

				//currentRect = clicked;															//Button Hover sprite.
			}
		}

		if (isInteractible)
		{
			listener->OnEventCall(this, ui_event);
		}
	}
	
	return;
}

void UI_InputBox::DrawInputBoxElements()
{
	background.Draw();
	text.Draw();
	DrawCursor();
}

void UI_InputBox::DrawCursor()
{
	if (this->isVisible && IsFocused())
	{
		App->render->DrawQuad(cursor.GetHitbox(), 255, 255, 255, 255);
	}
}

void UI_InputBox::UpdateInputBoxElementsPos()
{
	background.SetScreenPos(background.GetLocalPos() + GetScreenPos());
	background.SetHitbox({ background.GetScreenPos().x, background.GetScreenPos().y , background.GetHitbox().w, background.GetHitbox().h });

	text.SetScreenPos(text.GetLocalPos() + GetScreenPos());
	text.SetHitbox({ text.GetScreenPos().x, text.GetScreenPos().y , text.GetHitbox().w, text.GetHitbox().h });

	cursor.SetScreenPos({ cursor.GetLocalPos() + GetScreenPos() });
	cursor.SetHitbox({cursor.GetScreenPos().x, cursor.GetScreenPos().y, cursor.GetHitbox().w, cursor.GetHitbox().h});
}

void UI_InputBox::SetInputBoxElementsVisibility()
{
	if (this->isVisible != background.isVisible || this->isVisible != text.isVisible)
	{
		background.isVisible = this->isVisible;
		text.isVisible = this->isVisible;
	}

	if (this->isVisible != cursor.isVisible && IsFocused())
	{
		cursor.isVisible = this->isVisible;
	}
}

void UI_InputBox::CheckFocus()
{
	if (IsFocused())
	{
		text.ui_event = UI_Event::FOCUSED;
		cursor.isVisible = true;
		text.DeleteCurrentStringTex();
		text.SetCurrentStringTex(&App->gui->inputString);

		App->font->CalcSize(App->gui->inputString.GetString(), textWidth, textHeight, font);

		cursor.SetScreenPos({ cursor.GetScreenPos().x + textWidth, cursor.GetScreenPos().y });
		cursor.SetHitbox({ cursor.GetScreenPos().x, cursor.GetScreenPos().y, cursor.GetHitbox().w, cursor.GetHitbox().h });
	}
	else
	{
		text.ui_event = UI_Event::UNFOCUSED;
		cursor.isVisible = false;
		//text.SetCurrentStringTex();
	}
}