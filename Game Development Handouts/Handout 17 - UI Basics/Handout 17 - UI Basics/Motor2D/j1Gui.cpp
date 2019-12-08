#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Scene.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	ui_debug = false;
	escape = true;

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
	{
		PassFocus();
	}

	for (p2List_item<UI*>* element_iterator = elements.start; element_iterator != NULL; element_iterator = element_iterator->next)
	{
		switch (element_iterator->data->element)
		{
		case UI_Element::IMAGE:
			break;

		case UI_Element::TEXT:
			if (element_iterator->data->ui_event == UI_Event::CLICKED)
			{
				if (ui_debug == false)
				{
					ui_debug = true;
				}
				else
				{
					ui_debug = false;
				}
			}
			
			break;

		case UI_Element::BUTTON:
			
			//CheckFunctionality(element_iterator->data->elementCallback, element_iterator->data->ui_event);

			//if (element_iterator->data->ui_event == UI_Event::CLICKED)
			//{
			//	/*if (ui_debug == false)
			//	{
			//		ui_debug = true;
			//	}
			//	else
			//	{
			//		ui_debug = false;
			//	}*/
			//}

			break;
		}
	}
	
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{	
	//escape = true;
	
	for (p2List_item<UI*>* element_iterator = elements.start; element_iterator != NULL; element_iterator = element_iterator->next)
	{
		switch (element_iterator->data->element)
		{
		case UI_Element::IMAGE:
			element_iterator->data->Draw();
			break;

		case UI_Element::TEXT:
			element_iterator->data->Draw();
			break;

		case UI_Element::BUTTON:
			if (element_iterator->data->parent == NULL)
			{
				element_iterator->data->Draw();
			}
			/*else
			{

			}*/

			break;

		case UI_Element::SCROLLBAR:
			break;

		case UI_Element::INPUTBOX:
			break;
		}
	}

	Debug_UI();

	return escape;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	//Iterate the elements list and frees all allocated memory.
	for (p2List_item<UI*>* element_iterator = elements.start; element_iterator != NULL; element_iterator = element_iterator->next)
	{
		RELEASE(element_iterator->data);
	}

	elements.clear();

	return true;
}

// const getter for atlas
/*const*/ SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

//----------------------------------- UI ELEMENT CREATION METHODS -----------------------------------
UI* j1Gui::CreateElement(UI_Element element, int x, int y, SDL_Rect* idle, SDL_Rect* hover, SDL_Rect* clicked, p2SString* string)
{
	UI* elem = nullptr;

	switch (element)
	{
	case UI_Element::IMAGE:
		//elem = new UI_Image(element, x, y, *idle);					//In *rect the "*" is used to access rect pointer's data members.
		break;

	case UI_Element::TEXT:
		//elem = new UI_Text(element, x, y, *idle, string);
		break;

	case UI_Element::BUTTON:
		//elem = new UI_Button(element, x, y, *idle, *hover, *clicked);
		break;

	case UI_Element::SCROLLBAR:
		break;

	case UI_Element::INPUTBOX:
		break;
	}

	if (elem != nullptr)
	{
		elements.add(elem);
	}

	return elem;
}

UI* j1Gui::CreateImage(UI_Element element, int x, int y, SDL_Rect rect, UI_Image* imgCallback, UI* parent)
{
	UI* elem = nullptr;

	elem = new UI_Image(element, x, y, rect, imgCallback, parent);

	if (elem != nullptr)
	{
		elements.add(elem);
	}

	return elem;
}

UI* j1Gui::CreateText(UI_Element element, int x, int y, SDL_Rect hitbox, _TTF_Font* font, SDL_Color fontColour, UI_Text* textCallback, UI* parent, p2SString* string, p2SString* hoverString,
						p2SString* focusString, p2SString* leftClickString, p2SString* rightClickString)
{
	UI* elem = nullptr;

	elem = new UI_Text(element, x, y, hitbox, font, fontColour, textCallback, parent, string, hoverString, focusString, leftClickString, rightClickString);

	if (elem != nullptr)
	{
		elements.add(elem);
	}

	return elem;
}

UI* j1Gui::CreateButton(UI_Element element, int x, int y, UI_Button* buttonCallback, UI* parent, SDL_Rect* idle, SDL_Rect* hover, SDL_Rect* clicked)
{
	UI* elem = nullptr;

	elem = new UI_Button(element, x, y, buttonCallback, parent, idle, hover, clicked);

	if (elem != nullptr)
	{
		elements.add(elem);
	}

	return elem;
}

//--------------------------------- INPUT PROCESSING ---------------------------------
void j1Gui::OnEventCall(UI* element, UI_Event ui_event)
{
	if (element == App->scene->button/*debug_Button*/ && ui_event == UI_Event::CLICKED)		//If the pointer received is the UI_Button* button pointer of Scene.h and event = clicked. 
	{
		if (App->gui->ui_debug == false)
		{
			App->gui->ui_debug = true;
		}
		else
		{
			App->gui->ui_debug = false;
		}
		//return;
	}

	if (element == App->scene->escButton/*escape_Button*/ && ui_event == UI_Event::CLICKED)	//If the pointer received is the UI_Button* escbutton pointer of Scene.h and event = clicked.
	{
		escape = false;
		//return;
	}
}

void j1Gui::CheckFunctionality(UI_Callback callback, UI_Event ui_event)
{
	switch (callback)
	{
	case UI_Callback::NONE:
		break;

	case UI_Callback::UI_DEBUG_BUTTON:
		if (ui_event == UI_Event::CLICKED)
		{
			if (ui_debug == false)
			{
				ui_debug = true;
			}
			else
			{
				ui_debug = false;
			}
		}
		
		break;
	}
}

//----------------------------------- FOCUS METHOD -----------------------------------
void j1Gui::PassFocus()
{
	p2List_item<UI*>* element_iterator = elements.start;

	for (p2List_item<UI*>* element_iterator = elements.start; element_iterator != NULL; element_iterator = element_iterator->next)
	{
		if (element_iterator->data->interactible == true)
		{
			if (element_iterator->data->focused == false/* && element_iterator->data->hadFocus == false*/)
			{
				element_iterator->data->focused = true;
				//element_iterator->data->hadFocus = true;		//Had focus marks that a UI Element has alerady had focus, so it cannot have  focus until the next cycle.
				break;
			}
			else
			{
				element_iterator->data->focused = false;
				//element_iterator->data->hadFocus = false;
				//break;
			}

			/*if (element_iterator->data->hadFocus == true && element_iterator->next->data->hadFocus == true)
			{
				element_iterator->data->hadFocus = false;
				element_iterator->next->data->hadFocus = false;
			}*/
		}
		
	}
}

//----------------------------------- UI DEBUG METHOD -----------------------------------
void j1Gui::Debug_UI()
{
	if (ui_debug == true)
	{
		for (p2List_item<UI*>* element_iterator = elements.start; element_iterator != NULL; element_iterator = element_iterator->next)
		{	
			App->render->DrawQuad(element_iterator->data->GetHitbox(), 255, 0, 0, 255, false, false);
		}
	}
}

// class Gui ---------------------------------------------------

