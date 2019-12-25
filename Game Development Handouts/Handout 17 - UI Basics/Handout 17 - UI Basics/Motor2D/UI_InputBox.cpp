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
void UI_InputBox::DrawInputBoxElements()													// --------------------------------------------------------------------------
{
	background.Draw();
	text.Draw();
	DrawCursor();
}

void UI_InputBox::DrawCursor()																// --------------------------------------------------------------------------
{
	if (this->isVisible && IsFocused())
	{
		App->render->DrawQuad(cursor.GetHitbox(), cursorColour.r, cursorColour.g, cursorColour.b, cursorColour.a, true, false);
	}
}
// -----------------------------

// --- GETTERS / SETTERS -------
// --- This method returns the current input text length.
int UI_InputBox::TextLength()																// --------------------------------------------------------------------------
{
	return App->input->GetInputTextLength();
}

int UI_InputBox::GetCurrentCursorIndex()													// --------------------------------------------------------------------------
{
	return App->input->GetCursorIndex();
}
// -----------------------------

// --- INPUT BOX ELEMENTS STATE
void UI_InputBox::CheckInputBoxState()														// --------------------------------------------------------------------------
{
	UpdateInputBoxElementsPos();
	CheckFocus();
}

void UI_InputBox::UpdateInputBoxElementsPos()												// --------------------------------------------------------------------------
{
	if (this->GetScreenPos() != this->initialPosition)
	{
		background.SetScreenPos(background.GetLocalPos() + GetScreenPos());
		background.SetHitbox({ background.GetScreenPos().x
						, background.GetScreenPos().y
						, background.GetHitbox().w
						, background.GetHitbox().h });

		text.SetScreenPos(text.GetLocalPos() + GetScreenPos());
		text.SetHitbox({   text.GetScreenPos().x
						, text.GetScreenPos().y
						, text.GetHitbox().w
						, text.GetHitbox().h });

		cursor.SetScreenPos({ cursor.GetLocalPos() + GetScreenPos() });
		cursor.SetHitbox({ cursor.GetScreenPos().x
						, cursor.GetScreenPos().y
						, cursor.GetHitbox().w
						, cursor.GetHitbox().h });
	}
}

void UI_InputBox::CheckFocus()																// --------------------------------------------------------------------------
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

void UI_InputBox::CheckCursorInputs()														// --------------------------------------------------------------------------
{	
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)									// --------------------------------------------------------------------------
	{
		int prevIndex = GetCurrentCursorIndex();

		if (cursorPositions[prevIndex] != NULL)
		{
			SetCursorPosWithCursorIndex(prevIndex);

			LOG("CursorIndex Going Left %d", prevIndex);
			LOG("Cursor Position Going Left %d", cursorPositions[prevIndex]);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)			//See j1Input			// --------------------------------------------------------------------------
	{
		int nextIndex = GetCurrentCursorIndex();																	//THIS CURSOR---INDEX

		if (cursorPositions[nextIndex] != NULL && nextIndex <= TextLength() + 1)
		{
			SetCursorPosWithCursorIndex(nextIndex);

			LOG("CursorIndex Going Right %d", nextIndex);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN)		//See j1Input			// --------------------------------------------------------------------------
	{
		//currentIndex = GetCurrentCursorIndex();

		//if (GetCurrentCursorIndex() == TextLength())										//Only reset the cursor if the cursor is at the end of the text.
		//{	
		//	SetCursorPosWithTextWidth();

		//	cursorPositions[currentIndex] = cursor.GetHitbox().x;
		//}
		//else
		//{
		//	SetCursorPosWithCursorIndex(currentIndex);
		//}

		LOG("Cursor Index At IBx Delete: %d", currentIndex);
	}

	if (App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN)	//See j1Input				// --------------------------------------------------------------------------
	{
		currentIndex = GetCurrentCursorIndex();
		SetCursorPosWithCursorIndex(currentIndex);
		
		for (int i = 0; i < prevLength; i++)												//Cleans Up all previously recorded positions in the cursorPositions[] array.
		{
			cursorPositions[i] = NULL;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)								// --------------------------------------------------------------------------
	{
		currentIndex = GetCurrentCursorIndex();
		SetCursorPosWithCursorIndex(currentIndex);

		for (int i = 0; i < prevLength; i++)												//Cleans Up all previously recorded positions in the cursorPositions[] array..
		{
			cursorPositions[i] = NULL;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_HOME) == KEY_DOWN)		//See j1Input				// --------------------------------------------------------------------------
	{
		currentIndex = GetCurrentCursorIndex();

		SetCursorPosWithCursorIndex(currentIndex);
	}

	if (App->input->GetKey(SDL_SCANCODE_END) == KEY_DOWN)		//See j1Input				// --------------------------------------------------------------------------
	{
		currentIndex = GetCurrentCursorIndex();
		
		SetCursorPosWithCursorIndex(currentIndex);

		//cursorIndex = TextLength();

		LOG("END CURSOR INDEX %d", GetCurrentCursorIndex());
	}
}

void UI_InputBox::RefreshInputText()														// --------------------------------------------------------------------------
{
	text.DeleteCurrentStringTex();															//Sets to NULL the text's currentTex.
	text.RefreshTextInput(App->input->GetInputText());										//Refreshes text's string and texture with the string received from the input.

	if (prevLength != TextLength())															//If there has been a change in the string (Addition, deletion). Takes into account the length of the string in characters and cursorIndex.
	{
		RefreshCursorPos();

		prevLength = TextLength();															//As the text's length does not correspond with the cursor index, it is set to the new index.
		LOG("Current Length %d", TextLength());
	}
}

void UI_InputBox::RefreshCursorPos()														// --------------------------------------------------------------------------
{
	currentIndex = GetCurrentCursorIndex();													//Gets the current cursor index.

	if (currentIndex == TextLength())														//If cursor is at the text's end.
	{
		SetCursorPosWithTextWidth(App->input->GetInputText());								//Sets the cursor position as the cursor's origin position + textWidth.
	}
	else
	{
		ResetCursorPositions(currentIndex);													//Re-sets the cursor positions taking into account the inserted/deleted input text.

		SetCursorPosWithCursorIndex(currentIndex);											//If cursor is within the text, set the position according to the index.
	}

	cursorPositions[currentIndex] = cursor.GetHitbox().x;									//The current cursor hitbox's postion in the x axis is stored (cursor.pos().x + textWidth)
}

// --- Sets the cursor's position to the position stored in the cursorPositions array for the index passed as argument.
void UI_InputBox::SetCursorPosWithCursorIndex(int index)									// --------------------------------------------------------------------------
{
	cursor.SetHitbox({ cursorPositions[index]												//Sets the cursor hitbox's  position to the one stored at the cursor's index in the positions array. 
		, cursor.GetScreenPos().y
		, cursor.GetHitbox().w
		, cursor.GetHitbox().h });
}

// --- Sets the cursor's position to the cursor's origin position + textWidth.
void UI_InputBox::SetCursorPosWithTextWidth(const char* string)								// --------------------------------------------------------------------------												
{
	App->font->CalcSize(string, textWidth, textHeight, font);								//CalcSize calculates the current width and height of the current string/text.
	cursor.SetHitbox({ cursor.GetScreenPos().x + textWidth									//Sets the cursor's hitbox's position addind the new textWidth to it.
		, cursor.GetScreenPos().y
		, cursor.GetHitbox().w
		, cursor.GetHitbox().h });
}

void UI_InputBox::ResetCursorPositions(int index)											// --------------------------------------------------------------------------
{
	const char* inputString = App->input->GetInputText();									//Gets the current input string.
	char tmp[MAX_SIZE];																		//Buffer string that will be used to store different chunks of the string.
	
	for (int i = index; i <= TextLength(); i++)												//For i = index (cursor index), while index is less or equal to input_string length in characters.
	{
		for (int j = 0; j < i; j++)															//For j = 0, while j  is less than i.
		{
			tmp[j] = inputString[j];														//Stores in tmp the input string chunk before the current cursor's position.
		}

		tmp[i] = '\0';																		//Every element from [i] to the right (right of the cursor position) is deleted. Cleans the string.

		SetCursorPosWithTextWidth(tmp);														//Sets the new cursor position for the given input_string chunk.
		cursorPositions[i] = cursor.GetHitbox().x;											//Stores the new cursor position for index [i] in the positions array.
	}
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