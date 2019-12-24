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

	// --- Input Box Elements
	background = UI_Image(UI_Element::IMAGE, x, y, hitbox, true, false, false, this);
	text = UI_Text(UI_Element::TEXT, x + textOffset.x, y + textOffset.y, hitbox, font, fontColour, true, false, false, this, defaultString);
	cursor = UI_Image(UI_Element::IMAGE, x + textOffset.x, y + textOffset.y, cursorSize, false, false, false, this);
	
	// --- Text Variables
	this->font = font;														//Sets the UI input box font to the one being passed as argument.
	this->cursorColour = cursorColour;										//Sets the cursor colour to the cursor colour being passed as argument. (See DrawCursor())
	this->textOffset = textOffset;											//Sets the text offset to the text offset being passed as arguments.
	this->textWidth = 0;													//As the initial input text will be empty, text width is set to 0.
	this->textHeight = 0;													//As the initial input text will be empty, text height is set to 0. 
	this->prevLength = 0;
	//this->cursorIndex = 0;													//As the initial input text will be empty, cursorIndex is set to 0.
	this->cursorPositions[/*cursorIndex*/ 0] = cursor.GetScreenPos().x;			//As the initial input text will be empty, the first cursor position (cursor index 0) will be the cursor's origin position.
	
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
		//int prevIndex = cursorIndex - 1;
		int prevIndex = App->input->GetCursorIndex();

		if (cursorPositions[prevIndex] != NULL && prevIndex != 0)
		{
			cursor.SetHitbox({ /*cursor.GetHitbox().x -*/ cursorPositions[prevIndex],
				cursor.GetScreenPos().y,
				cursor.GetHitbox().w,
				cursor.GetHitbox().h });

			LOG("CursorIndex Going Left %d", prevIndex);
			LOG("Cursor Position Going Left %d", cursorPositions[prevIndex]);

			//cursorIndex = prevIndex;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		//int nextIndex = cursorIndex + 1;
		int nextIndex = App->input->GetCursorIndex();																	//THIS CURSOR---INDEX

		if (cursorPositions[nextIndex] != NULL && nextIndex <= TextLength() + 1)
		{
			cursor.SetHitbox({ /*cursor.GetHitbox().x +*/ cursorPositions[nextIndex],
				cursor.GetScreenPos().y,
				cursor.GetHitbox().w,
				cursor.GetHitbox().h });

			LOG("CursorIndex Going Right %d", nextIndex);
			//cursorIndex = nextIndex;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN)
	{
		currentCursorIndex = App->input->GetCursorIndex();

		if (currentCursorIndex == TextLength())							//Only reset the cursor if the cursor is at the end of the text.
		{
			App->font->CalcSize(App->input->GetInputText(), textWidth, textHeight, font);
			cursor.SetHitbox({ cursor.GetScreenPos().x + textWidth, cursor.GetScreenPos().y, cursor.GetHitbox().w, cursor.GetHitbox().h });
			
			cursorPositions[currentCursorIndex] = cursor.GetHitbox().x;
		}
		else
		{
			cursor.SetHitbox({ cursorPositions[currentCursorIndex], cursor.GetScreenPos().y, cursor.GetHitbox().w, cursor.GetHitbox().h });
		}

		LOG("Cusor Index At IBx Delete: %d", currentCursorIndex);
	}

	if (App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN)				//If the delete key is pressed, all the text will be deleted (See j1Input) and the cursorPositions array will be emptied.
	{
		//int currentLength = TextLength();

		currentCursorIndex = App->input->GetCursorIndex();
		cursor.SetHitbox({ cursorPositions[currentCursorIndex], cursor.GetScreenPos().y, cursor.GetHitbox().w, cursor.GetHitbox().h });
		
		for (int i = 0; i < prevLength; i++)								//Cleans Up all previously recorded positions.
		{
			cursorPositions[i] = NULL;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{

	}

	if (App->input->GetKey(SDL_SCANCODE_HOME) == KEY_DOWN)		//See j1Input
	{
		currentCursorIndex = App->input->GetCursorIndex();
		cursor.SetHitbox({ cursorPositions[currentCursorIndex], cursor.GetScreenPos().y, cursor.GetHitbox().w, cursor.GetHitbox().h });

		//cursorIndex = App->input->GetCursorIndex();
		//cursor.SetHitbox({ cursorPositions[cursorIndex], cursor.GetScreenPos().y, cursor.GetHitbox().w, cursor.GetHitbox().h });
	}

	if (App->input->GetKey(SDL_SCANCODE_END) == KEY_DOWN)		//See j1Input
	{
		currentCursorIndex = App->input->GetCursorIndex();
		
		if (currentCursorIndex != 0)
		{
			cursor.SetHitbox({ cursorPositions[currentCursorIndex - 1], cursor.GetScreenPos().y, cursor.GetHitbox().w, cursor.GetHitbox().h });
		}
		else
		{
			cursor.SetHitbox({ cursorPositions[currentCursorIndex], cursor.GetScreenPos().y, cursor.GetHitbox().w, cursor.GetHitbox().h });
		}

		//cursorIndex = TextLength();
		//cursor.SetHitbox({ cursorPositions[cursorIndex], cursor.GetScreenPos().y, cursor.GetHitbox().w, cursor.GetHitbox().h });

		LOG("END CURSOR INDEX %d", App->input->GetCursorIndex());
	}
}

void UI_InputBox::RefreshInputText()
{
	text.DeleteCurrentStringTex();																							//Sets to NULL the text's currentTex.
	text.RefreshCharTextInput(App->input->GetInputText());																	//Refreshes text's string and texture with the string received from the input.

	if (prevLength != TextLength())																							//If there has been a change in the string (Addition, deletion). Takes into account the length of the string in characters and cursorIndex.
	{
		currentCursorIndex = App->input->GetCursorIndex();		
		//cursorPositions[currentCursorIndex] = cursor.GetHitbox().x;															//Instead of storing the full text width, what is stored is the width that the addition/deletion has brought.
		
		if (prevLength <= TextLength())
		{
			prevTextWidth = textWidth;																							//Keeps record of the previous width of the text before there was an addition or deletion.
			App->font->CalcSize(App->input->GetInputText(), textWidth, textHeight, font);										//CalcSize calculates the current width and height of the current string/text.
			
			cursor.SetHitbox({ cursor.GetScreenPos().x + textWidth, cursor.GetScreenPos().y, cursor.GetHitbox().w, cursor.GetHitbox().h });		//Sets the cursor's hitbox's position addind the new textWidth to it.
			//cursorPositions[currentCursorIndex] = cursor.GetHitbox().x;
		}
		
		cursorPositions[currentCursorIndex] = cursor.GetHitbox().x;

		prevLength = TextLength();																		//As the text's length does not correspond with the cursor index, it is set to the new index.
		LOG("Current Length %d", TextLength());

		//cursorIndex++;
		//cursorIndex = prevLength;

		//cursorPositions[cursorIndex] = cursor.GetHitbox().x;																//Instead of storing the full text width, what is stored is the width that the addition/deletion has brought.
		//cursorPositions[cursorIndex] = textWidth;																			//As the cursor index has changed, a new position is stored in the cursor position array. Should it go backwards (deletion) the prevous position at that index would be overwritten.
		//cursorPositions[cursorIndex] = textWidth - prevTextWidth;															//Instead of storing the full text width, what is stored is the width that the addition/deletion has brought.
		//cursorPositions[prevLength] = cursor.GetHitbox().x;																//Instead of storing the full text width, what is stored is the width that the addition/deletion has brought.

		//cursor.SetScreenPos({ cursor.GetScreenPos().x + textWidth, cursor.GetScreenPos().y });							//Sets the cursor position adding to it the width of the text.
		//cursor.SetHitbox({ cursor.GetScreenPos().x + textWidth, cursor.GetScreenPos().y, cursor.GetHitbox().w, cursor.GetHitbox().h });		//Sets the cursor's hitbox's position addind the new textWidth to it.
	}
}

// --- This method returns the current input text length.
int UI_InputBox::TextLength()
{
	return App->input->GetInputTextLength();
}
// ----------------------------------------------

void UI_InputBox::SetInputBoxVisibility()
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