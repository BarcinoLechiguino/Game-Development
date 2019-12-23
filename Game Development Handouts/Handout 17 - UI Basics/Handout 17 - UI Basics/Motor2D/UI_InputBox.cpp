#include "j1App.h"
#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Gui.h"
//#include "UI.h"
#include "UI_Image.h"
#include "UI_Text.h"
#include "UI_InputBox.h"

#include "p2DynArray.h"

UI_InputBox::UI_InputBox(UI_Element element, int x, int y, SDL_Rect hitbox, _TTF_Font* font, SDL_Color fontColour, SDL_Rect cursorSize, SDL_Color cursorColour, iPoint textOffset,
		bool isVisible, bool isInteractible, bool isDraggable, UI* parent, p2SString* defaultString) : UI(element, x, y, hitbox, parent)
{
	tex = App->gui->GetAtlas();
	
	if (/*this->isInteractible*/ isInteractible)							//If the Input Box element is interactible.
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

	// ----------------------------- INPUT BOX VARIABLES ---------------------------------
	// --- Input Box Flags
	this->isVisible = isVisible;											//Sets the isVisible flag of the input box to the one passed as argument.
	this->isInteractible = isInteractible;									//Sets the isInteractible flag of the input box to the one passed as argument.
	this->isDraggable = isDraggable;										//Sets the isDraggable flag of the input box to the one passed as argument.

	prevMousePos = iPoint(0, 0);											//Initializes prevMousePos for this UI Element. Safety measure to avoid weird dragging behaviour.

	initialPosition = GetScreenPos();	//
	
	// --- Text Variables
	this->font = font;														//Sets the UI input box font to the one being passed as argument.
	this->cursorColour = cursorColour;										//Sets the cursor colour to the cursor colour being passed as argument. (See DrawCursor())
	this->textOffset = textOffset;											//Sets the text offset to the text offset being passed as arguments.
	this->textWidth = 0;													//As the initial input text will be empty, text width is set to 0.
	this->textHeight = 0;													//As the initial input text will be empty, text height is set to 0. 
	this->prevLength = 0;
	this->cursorIndex = 0;													//As the initial input text will be empty, cursorIndex is set to 0.
	this->cursorPositions[cursorIndex] = 0;									//As the initial input text will be empty, the first cursor position (cursor index 0) will be 0.

	// --- Input Box Elements
	background = UI_Image(UI_Element::IMAGE, x, y, hitbox, true, false, false, this);
	text = UI_Text(UI_Element::TEXT, x + textOffset.x, y + textOffset.y, hitbox, font, fontColour, true, false, false, this, defaultString);
	cursor = UI_Image(UI_Element::IMAGE, x + textOffset.x, y + textOffset.y, cursorSize, false, false, false, this);

	//text.SetPreviousInputString(defaultString);
	// --------------------------------------------------------------------------------------
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
	if (isVisible)
	{
		CheckInputBoxState();
		
		GetMousePos();																				//Gets the mouse's position on the screen.

		// --- IDLE EVENT
		if (!IsHovered())																			//If the mouse is not on the text.
		{
			ui_event = UI_Event::IDLE;
		}

		if (isInteractible)																			//If the Text element is interactible.
		{
			// --- HOVER EVENT
			if ((IsHovered() && IsForemostElement()) /*|| IsFocused()*/)							//If the mouse is on the text.
			{
				ui_event = UI_Event::HOVER;
			}

			
			// --- FOCUS EVENT
			//CheckFocus();
			
			// --- CLICKED EVENT (Left Click)
			if (IsHovered() && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)			//If the mouse is on the text and the left mouse button is pressed.
			{
				prevMousePos = GetMousePos();														//Sets the initial position where the mouse was before starting to drag the element.
				initialPosition = GetScreenPos();													//Sets initialPosition with the current position at mouse KEY_DOWN.
				App->gui->focusedElement = this;													//Set the focus on the Input Box element when it is clicked.
			}

			if (!IsHovered() && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)		//Unfocus when the mouse clicks outside the input box.
			{
				if (App->gui->focusedElement == this)
				{
					App->gui->focusedElement = nullptr;
				}
			}

			if (IsHovered() && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)		//If the mouse is on the text and the left mouse button is being pressed.
			{
				if (IsForemostElement() /*|| IsFocused()*/)											//If the UI Text element is the foremost element under the mouse. 
				{
					ui_event = UI_Event::CLICKED;

					if (ElementCanBeDragged())														//If the UI Text element is draggable and is the foremost element under the mouse. 
					{
						DragElement();																//The Text element is dragged.

						CheckElementChilds();														//Checks if this Text element has any childs and updates them in consequence.

						prevMousePos = GetMousePos();												//Updates prevMousePos so it can be dragged again next frame.
					}
				}
			}

			// --- UNCLICKED EVENT (Left Click)
			if (IsHovered() == true && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)	//If the mouse is on the text and the left mouse button is released.
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

// --- DRAW INPUT BOX ELEMENTS
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
		App->render->DrawQuad(cursor.GetHitbox(), cursorColour.r, cursorColour.g, cursorColour.b, cursorColour.a, true, false);
	}
}
// -----------------------------

// --- INPUT BOX ELEMENTS STATE
void UI_InputBox::CheckInputBoxState()
{
	UpdateInputBoxElementsPos();
	CheckFocus();
}

void UI_InputBox::UpdateInputBoxElementsPos()
{
	if (this->GetScreenPos() != this->initialPosition)
	{
		background.SetScreenPos(background.GetLocalPos() + GetScreenPos());
		background.SetHitbox({ background.GetScreenPos().x, background.GetScreenPos().y , background.GetHitbox().w, background.GetHitbox().h });

		text.SetScreenPos(text.GetLocalPos() + GetScreenPos());
		text.SetHitbox({ text.GetScreenPos().x, text.GetScreenPos().y , text.GetHitbox().w, text.GetHitbox().h });

		cursor.SetScreenPos({ cursor.GetLocalPos() + GetScreenPos() });
		cursor.SetHitbox({ cursor.GetScreenPos().x, cursor.GetScreenPos().y, cursor.GetHitbox().w, cursor.GetHitbox().h });
	}
}

void UI_InputBox::CheckFocus()
{
	if (IsFocused() && isVisible)
	{
		text.ui_event = UI_Event::FOCUSED;
		cursor.isVisible = true;
		
		CheckCursorInputs();
		RefreshInputText();
	}
	else
	{
		text.ui_event = UI_Event::UNFOCUSED;
		cursor.isVisible = false;
		//text.SetCurrentStringTex();
	}
}

void UI_InputBox::CheckCursorInputs()
{	
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		int prevIndex = cursorIndex - 1;

		if (cursorPositions[prevIndex] != NULL || cursorIndex != 0)
		{
			//LOG("Cursor Prev Hitbox: { %d %d %d %d }", cursor.GetHitbox().x, cursor.GetHitbox().y, cursor.GetHitbox().w, cursor.GetHitbox().h);

			cursor.SetHitbox({ /*cursor.GetHitbox().x -*/ cursorPositions[prevIndex],
				cursor.GetScreenPos().y,
				cursor.GetHitbox().w,
				cursor.GetHitbox().h });

			//LOG("Cursor Next Hitbox: { %d %d %d %d }", cursor.GetHitbox().x, cursor.GetHitbox().y, cursor.GetHitbox().w, cursor.GetHitbox().h);

			LOG("CursorIndex %d", cursorIndex);
			LOG("Cursor Position %d", cursorPositions[prevIndex]);

			cursorIndex = prevIndex;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		int nextIndex = cursorIndex + 1;

		if (cursorPositions[nextIndex] != NULL || cursorIndex < App->input->InputTextLength())
		{
			cursor.SetHitbox({ /*cursor.GetHitbox().x +*/ cursorPositions[nextIndex],
				cursor.GetScreenPos().y,
				cursor.GetHitbox().w,
				cursor.GetHitbox().h });

			LOG("CursorIndex %d", cursorIndex);
			cursorIndex = nextIndex;
		}
	}
}

void UI_InputBox::RefreshInputText()
{
	text.DeleteCurrentStringTex();																							//Sets to NULL the text's currentTex.
	text.RefreshCharTextInput(App->input->GetInputText());																	//Refreshes text's string and texture with the string received from the input.

	if (App->input->InputTextLength() != prevLength)																		//If there has been a change in the string (Addition, deletion). Takes into account the length of the string in characters and cursorIndex.
	{
		prevLength = App->input->InputTextLength();																			//As the text's length does not correspond with the cursor index, it is set to the new index.
		cursorIndex++;

		if (cursorIndex < prevLength) { cursorIndex = prevLength; }

		prevTextWidth = textWidth;																							//Keeps record of the previous width of the text before there was an addition or deletion.

		App->font->CalcSize(App->input->GetInputText(), textWidth, textHeight, font);										//CalcSize calculates the current width and height of the current string/text.

		//cursorPositions[cursorIndex] = textWidth;																			//As the cursor index has changed, a new position is stored in the cursor position array. Should it go backwards (deletion) the prevous position at that index would be overwritten.
		//cursorPositions[cursorIndex] = textWidth - prevTextWidth;															//Instead of storing the full text width, what is stored is the width that the addition/deletion has brought.
		cursorPositions[cursorIndex] = cursor.GetHitbox().x;															//Instead of storing the full text width, what is stored is the width that the addition/deletion has brought.


		//cursor.SetScreenPos({ cursor.GetScreenPos().x + textWidth, cursor.GetScreenPos().y });							//Sets the cursor position adding to it the width of the text.
		cursor.SetHitbox({ cursor.GetScreenPos().x + textWidth, cursor.GetScreenPos().y, cursor.GetHitbox().w, cursor.GetHitbox().h });		//Sets the cursor's hitbox's position addind the new textWidth to it.
	}


	//if (App->input->InputTextLength() != cursorIndex)																		//If there has been a change in the string (Addition, deletion). Takes into account the length of the string in characters and cursorIndex.
	//{
	//	cursorIndex = App->input->InputTextLength();																		//As the text's length does not correspond with the cursor index, it is set to the new index.
	//	prevTextWidth = textWidth;																							//Keeps record of the previous width of the text before there was an addition or deletion.

	//	App->font->CalcSize(App->input->GetInputText(), textWidth, textHeight, font);										//CalcSize calculates the current width and height of the current string/text.

	//	cursorPositions[cursorIndex] = textWidth;																			//As the cursor index has changed, a new position is stored in the cursor position array. Should it go backwards (deletion) the prevous position at that index would be overwritten.
	//	//cursorPositions[cursorIndex] = textWidth - prevTextWidth;															//Instead of storing the full text width, what is stored is the width that the addition/deletion has brought.


	//	//cursor.SetScreenPos({ cursor.GetScreenPos().x + textWidth, cursor.GetScreenPos().y });							//Sets the cursor position adding to it the width of the text.
	//	cursor.SetHitbox({ cursor.GetScreenPos().x + textWidth, cursor.GetScreenPos().y, cursor.GetHitbox().w, cursor.GetHitbox().h });		//Sets the cursor's hitbox's position addind the new textWidth to it.
	//}
	
	//LOG("Cursor Prev to Refresh Hitbox: { %d %d %d %d }", cursor.GetHitbox().x, cursor.GetHitbox().y, cursor.GetHitbox().w, cursor.GetHitbox().h);
	//LOG("Cursor Refresh Hitbox: { %d %d %d %d }", cursor.GetHitbox().x, cursor.GetHitbox().y, cursor.GetHitbox().w, cursor.GetHitbox().h);

	/*text.DeleteCurrentStringTex();
	text.SetInputStringTex(&App->gui->inputString);

	if (App->gui->inputString.Length() != letterCount)
	{
		App->font->CalcSize(App->gui->inputString.GetString(), textWidth, textHeight, font);

		//cursor.SetScreenPos({ cursor.GetScreenPos().x + textWidth, cursor.GetScreenPos().y });
		cursor.SetHitbox({ cursor.GetScreenPos().x + textWidth, cursor.GetScreenPos().y, cursor.GetHitbox().w, cursor.GetHitbox().h });

		letterCount = App->gui->inputString.Length();

		//prevTextWidth = textWidth;
	}*/
}
// ----------------------------------------------

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