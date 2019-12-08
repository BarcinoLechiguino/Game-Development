#include "j1App.h"
#include "j1Fonts.h"
#include "j1Gui.h"
#include "UI.h"
#include "UI_Text.h"

UI_Text::UI_Text(UI_Element type, int x, int y, SDL_Rect hitbox, _TTF_Font* font, SDL_Color fontColour, UI_Text* elementCallback, UI* parent, p2SString* string, p2SString* hoverString,
				p2SString* focusString, p2SString* leftClickString, p2SString* rightClickString) : UI(UI_Element::TEXT, x, y, hitbox, parent),
				idleTex(nullptr), hoverTex(nullptr), focusTex(nullptr), leftClickTex(nullptr), rightClickTex(nullptr)
{	
	//Loading all strings. Print() generates a texture with the given string with the a font and a colour.
	if (string != NULL)
	{
		idleTex = App->font->Print(string->GetString(), fontColour, font);					//Creates the texture for the idle state.
	}

	if (hoverString != NULL)
	{
		hoverTex = App->font->Print(hoverString->GetString(), fontColour, font);			//Creates the texture for the hover state.
	}
	
	if (focusString != NULL)
	{
		focusTex = App->font->Print(focusString->GetString(), fontColour, font);			//Creates the texture for the focused state.
	}
	
	if (leftClickString != NULL)
	{
		leftClickTex = App->font->Print(leftClickString->GetString(), fontColour, font);	//Creates the texture for the left-clicked state.
	}

	if (rightClickString != NULL)
	{
		rightClickTex = App->font->Print(rightClickString->GetString(), fontColour, font);	//Crates the texture for the right_clicked state.
	}

	//interactible = true;																	//A UI_Text type of UI Element will never be interactible.
	interactible = false;	//Temporal measure to debug the focus.
	//focused = false;

	textCallback = elementCallback;
}

bool UI_Text::Draw()
{	
	GetMousePos();

	bool hovered = CheckMousePos();

	if (hovered == false)																		//If the mouse is not on the text.
	{
		ui_event = UI_Event::IDLE;
		currentTex = idleTex;																	//Blit the idle text.
	}

	if (hovered == true || focused == true)													//If the mouse is on the text.
	{
		ui_event = UI_Event::HOVER;

		if (hoverTex != NULL)
		{
			currentTex = hoverTex;																//Blit the hover text.
		}
	}

	if (hovered == true && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)			//If the mouse is on the button and the left mouse button is pressed.
	{
		ui_event = UI_Event::CLICKED;

		if (leftClickTex != NULL)
		{
			currentTex = leftClickTex;															//Blit the left click text.
		}
	}

	if (hovered == true && App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)	//If the mouse is on the button and the right mouse button is pressed.
	{
		ui_event = UI_Event::CLICKED;
		
		if (rightClickTex != NULL)
		{
			currentTex = rightClickTex;														//Blit the right click text.
		}
	}

	BlitElement(currentTex, GetPosition().x, GetPosition().y, NULL);

	return true;
}